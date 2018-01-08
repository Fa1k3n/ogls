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

ACTION_TEMPLATE(
    StrCpyToArg,
    HAS_1_TEMPLATE_PARAMS(int, k),
    AND_1_VALUE_PARAMS(str)) {
  strcpy(std::tr1::get<k>(args), str);
}

class ShaderTest : public ::testing::Test {
public:
	NiceMock<GLMock> m_glMock;

protected:
	void SetUp() {
		ON_CALL(m_glMock, gl_CreateShader(_)).WillByDefault(Return(1));
		ON_CALL(m_glMock, gl_GetError()).WillByDefault(Return(GL_NO_ERROR));
		ON_CALL(m_glMock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillByDefault(SetArgPointee<2>(1));
		ON_CALL(m_glMock, gl_GetShaderInfoLog(_, _, _, _)).WillByDefault(StrCpyToArg<3>("MOCK ERROR"));
	}
};

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

TEST_F(ShaderTest, createBasicShader)
{
	EXPECT_CALL(m_glMock, gl_CreateShader(GL_VERTEX_SHADER)).WillOnce(Return(1));
	EXPECT_CALL(m_glMock, gl_CreateShader(GL_FRAGMENT_SHADER)).WillOnce(Return(2));

	ASSERT_EQ(ogls::Shader(GL_VERTEX_SHADER).type(), GL_VERTEX_SHADER);
	ASSERT_EQ(ogls::Shader(GL_FRAGMENT_SHADER).type(), GL_FRAGMENT_SHADER);
}

TEST_F(ShaderTest, createUnknownShaderType) {
	ASSERT_THROW(ogls::Shader(1), ogls::ShaderException);
}

TEST_F(ShaderTest, failureToCreateShaderWillThrow) {
	EXPECT_CALL(m_glMock, gl_CreateShader(GL_VERTEX_SHADER)).WillOnce(Return(0));
	ASSERT_THROW(ogls::Shader(GL_VERTEX_SHADER), ogls::ShaderException);
}

TEST_F(ShaderTest, addSourceToShader) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar");
}

TEST_F(ShaderTest, possibleToAddManySources) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 2, SourceMatches(EXP_SRCS("foo bar", "baz")), NULL));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").addSource("baz");
}

TEST_F(ShaderTest, compileShader) {
	EXPECT_CALL(m_glMock, gl_CompileShader(1));
	EXPECT_CALL(m_glMock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile();
}

TEST_F(ShaderTest, compileShaderThrowsErrorIfFailed) {
	EXPECT_CALL(m_glMock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillOnce(SetArgPointee<2>(0));
	try {
		ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile();
	} catch (ogls::ShaderException e) {
		ASSERT_STREQ(e.what(), "MOCK ERROR");
	}
}

TEST_F(ShaderTest, easyCreation) {
	ASSERT_EQ(ogls::VertexShader().type(), GL_VERTEX_SHADER);
	ASSERT_EQ(ogls::FragmentShader().type(), GL_FRAGMENT_SHADER);
}

TEST_F(ShaderTest, solidColorShaderRGBConstruct) {
	ogls::ColorRGB c(1.0f, 0.1f, 0.1f);
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, _, SourceContains("vec4(1.000f, 0.100f, 0.100f, 1.0f);"), NULL));
	ogls::SolidColorShader scs(c);
	ASSERT_EQ(scs.type(), GL_FRAGMENT_SHADER);
	ASSERT_EQ(scs.color(), c);
}

TEST_F(ShaderTest, solidColorShaderEnumConstruct) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, _, SourceContains("vec4(1.000f, 0.000f, 0.000f, 1.0f);"), NULL));
	ogls::SolidColorShader scs(ogls::RED);
	ASSERT_EQ(scs.type(), GL_FRAGMENT_SHADER);
	ASSERT_EQ(scs.color(), ogls::ColorRGB(1.0f, 0.0f, 0.0f));
}

TEST_F(ShaderTest, addSourceFromConstructor) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL)).Times(2);
	ogls::FragmentShader("foo bar");
	ogls::VertexShader("foo bar");
}

TEST_F(ShaderTest, setVersion) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, _, SourceContains("#version 330 core"), NULL));
	ogls::Shader(GL_VERTEX_SHADER).setVersion(3, 30);
}

TEST_F(ShaderTest, errorCanBeThrownWhenAddingShader) {
	EXPECT_CALL(m_glMock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION)).WillOnce(Return(3));
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar"), ogls::ShaderException);
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar"), ogls::ShaderException);
	EXPECT_THROW(ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar"), ogls::ShaderException);
}

TEST_F(ShaderTest, compileIsOnlyCalledWhenNeeded) {
	EXPECT_CALL(m_glMock, gl_CompileShader(1)).Times(1).RetiresOnSaturation();
	auto shdr = ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile();
	shdr.compile();

	// But if a source was added then it will be called again
	EXPECT_CALL(m_glMock, gl_CompileShader(1)).Times(2);
	auto shdr2 = ogls::Shader(GL_VERTEX_SHADER).addSource("foo bar").compile();
	shdr2.addSource("baz fiz");
	shdr2.compile();
}

TEST_F(ShaderTest, nothingIsCompiledIfNoSourcesHaveBeenAdded) {
	EXPECT_CALL(m_glMock, gl_CompileShader(1)).Times(0);
	auto shdr = ogls::Shader(GL_VERTEX_SHADER).compile();
}

TEST_F(ShaderTest, shaderExceptionHasReadableErrors) {
	try {
		ogls::Shader(2);
	} catch (ogls::ShaderException e) {
		ASSERT_STREQ("Not a valid shader type", e.what());
	}
}

TEST_F(ShaderTest, addStream) {
	std::stringstream ss;
	ss << "foo bar";
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));
	ogls::Shader(GL_VERTEX_SHADER).addSource(ss);
}

TEST_F(ShaderTest, possibleToUseStreamOperator) {
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 1, SourceMatches(EXP_SRCS("foo bar")), NULL));
	EXPECT_CALL(m_glMock, gl_ShaderSource(_, 2, SourceMatches(EXP_SRCS("foo bar", "baz")), NULL));

	auto shdr = ogls::Shader(GL_FRAGMENT_SHADER);
	shdr << "foo bar" << "baz";

}