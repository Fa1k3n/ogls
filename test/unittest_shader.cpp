#include "glmock.hpp"
#include "gtest/gtest.h"

#include "shader.hpp"

#include <sstream>
#include <string>

using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::SetArgPointee;

TEST(ShaderTest, createBasicShader)
{
	GLMock mock;
	EXPECT_CALL(mock, gl_CreateShader(GL_VERTEX_SHADER)).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_CreateShader(GL_FRAGMENT_SHADER)).WillOnce(Return(2));

	ogls::Shader verts(GL_VERTEX_SHADER);
	ASSERT_EQ(verts.type(), GL_VERTEX_SHADER);

	ogls::Shader frags(GL_FRAGMENT_SHADER);
	ASSERT_EQ(frags.type(), GL_FRAGMENT_SHADER);
}

TEST(ShaderTest, createUnknownShaderType) {
	ASSERT_THROW(ogls::Shader shader(1), ogls::ShaderException);
}

TEST(ShaderTest, failureToCreateShaderWillThrow) {
	NiceMock<GLMock> mock;
	ASSERT_THROW(ogls::Shader shader(1), ogls::ShaderException);
}

MATCHER_P(SourceMatches, data, "") {
	int off = 0;
	for(auto exp : data) {
		if (strcmp(arg[off++], exp) != 0) {
			return false;
		}
	}
	return true;
}

MATCHER_P(SourceContains, data, "") {
	std::string str(*arg);
	if (str.find(data) != std::string::npos) {
		return true;
	}
	return false;
}

#define EXP_SRCS(...) std::vector<const char*>({__VA_ARGS__})

TEST(ShaderTest, addSourceToShader) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));

	ogls::Shader vert(GL_VERTEX_SHADER);
	vert.addSource("foo bar");
}

TEST(ShaderTest, callsAreChainable) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar");
}

TEST(ShaderTest, possibleToAddManySources) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, 1, _, NULL));
	EXPECT_CALL(mock, gl_ShaderSource(_, 2, SourceMatches(EXP_SRCS("foo bar", "baz")), NULL));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").addSource("baz");
}

TEST(ShaderTest, compileShader) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CompileShader(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile();
}

TEST(ShaderTest, compileShaderThrowsErrorIfFailed) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CompileShader(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillOnce(SetArgPointee<2>(0));
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile(), ogls::ShaderException);
}

TEST(ShaderTest, easyCreation) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	ogls::VertexShader vert;
	ASSERT_EQ(vert.type(), GL_VERTEX_SHADER);

	ogls::FragmentShader frag;
	ASSERT_EQ(frag.type(), GL_FRAGMENT_SHADER);
}

TEST(ShaderTest, solidColorShaderRGBConstruct) {
	NiceMock<GLMock> mock;
	ogls::ColorRGB c(1.0f, 0.1f, 0.1f);
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, _, SourceContains("vec4(1.000f, 0.100f, 0.100f, 1.0f);"), NULL));
	ogls::SolidColorShader scs(c);
	ASSERT_EQ(scs.type(), GL_FRAGMENT_SHADER);
	ASSERT_EQ(scs.color(), c);
}

TEST(ShaderTest, solidColorShaderEnumConstruct) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, _, SourceContains("vec4(1.000f, 0.000f, 0.000f, 1.0f);"), NULL));
	ogls::SolidColorShader scs(ogls::RED);
	ASSERT_EQ(scs.type(), GL_FRAGMENT_SHADER);
	ASSERT_EQ(scs.color(), ogls::ColorRGB(1.0f, 0.0f, 0.0f));
}

TEST(ShaderTest, addSourceFromConstructor) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL)).Times(2);
	ogls::FragmentShader("foo bar");
	ogls::VertexShader("foo bar");
}

TEST(ShaderTest, setVersion) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_ShaderSource(_, _, SourceContains("#version 330 core"), NULL));
	ogls::Shader(GL_VERTEX_SHADER).setVersion(3, 30);
}

TEST(ShaderTest, errorCanBeThrownWhenAddingShader) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION));
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar"), ogls::ShaderException);
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar"), ogls::ShaderException);
}
