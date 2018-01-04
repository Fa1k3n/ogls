#include "glmock.hpp"
#include "gtest/gtest.h"

#include "program.hpp"
#include "shader.hpp"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
using ::testing::NiceMock;

TEST(ProgramTest, createBasicProgram)
{
	GLMock mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	ogls::Program();
}

TEST(ProgramTest, failCreateBasicProgram)
{
	GLMock mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(0));
	ASSERT_THROW(ogls::Program(), ogls::ProgramException);
}

TEST(ProgramTest, addShader) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_CreateShader(GL_FRAGMENT_SHADER)).WillOnce(Return(3));
	EXPECT_CALL(mock, gl_AttachShader(1, 3));
	EXPECT_CALL(mock, gl_GetError()).WillRepeatedly(Return(GL_NO_ERROR));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar"));
}

TEST(ProgramTest, addShaderWithFailure) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateShader(GL_FRAGMENT_SHADER)).WillRepeatedly(Return(3));
	EXPECT_CALL(mock, gl_AttachShader(1, 3)).Times(3);
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto shdr = ogls::FragmentShader().addSource("foo bar");
	EXPECT_CALL(mock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION)).WillOnce(Return(2));

	EXPECT_THROW(ogls::Program().addShader(shdr), ogls::ProgramException);
	EXPECT_THROW(ogls::Program().addShader(shdr), ogls::ProgramException);
	EXPECT_THROW(ogls::Program().addShader(shdr), ogls::ProgramException);
}

TEST(ProgramTest, linkBasicProgram) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_LinkProgram(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));

	auto p = ogls::Program();
	auto fs = ogls::FragmentShader();
	fs.addSource("foo bar");
	p.addShader(fs);
	p.link();
}

TEST(ProgramTest, failingToLinkThrowsException) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_LinkProgram(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(0));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar"));
	ASSERT_THROW(p.link(), ogls::ProgramException);
}

TEST(ProgramTest, callsAreChainable) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto frag = ogls::FragmentShader("foo bar");
	auto vert = ogls::VertexShader("foo bar");
	auto p = ogls::Program();
	p.addShader(frag).addShader(vert).link();
}

TEST(ProgramTest, useProgram) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	EXPECT_CALL(mock, gl_UseProgram(1));
	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar")).link().use();
}

TEST(ProgramTest, useProgramWithError) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar")).link();

	EXPECT_CALL(mock, gl_UseProgram(1)).Times(3);
	EXPECT_CALL(mock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION)).WillOnce(Return(2));
	EXPECT_THROW(p.use(), ogls::ProgramException);
	EXPECT_THROW(p.use(), ogls::ProgramException);
	EXPECT_THROW(p.use(), ogls::ProgramException);
}

TEST(ProgramTest, addTwoShadersOfSameTypeWillFail) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
	auto p = ogls::Program().addShader(ogls::FragmentShader().addSource("foo bar"));
	ASSERT_THROW(p.addShader(ogls::FragmentShader().addSource("foo baz")), ogls::ProgramException);
}

TEST(ProgramTest, setUniform) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetUniformLocation(1, "baz")).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_Uniform1f(1, 1.0f));
	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar")).link();
	p.setUniform<float>("baz", {1.0f});
}

TEST(ProgramTest, setUniform3) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetUniformLocation(1, "baz")).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_Uniform3f(1, 1.0f, 1.0f, 1.0f));
	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar")).link();
	p.setUniform<float>("baz", {1.0f, 1.0f, 1.0f});
}

TEST(ProgramTest, addUncompiledShaderWillCompileIt) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	EXPECT_CALL(mock, gl_CreateShader(GL_FRAGMENT_SHADER)).WillOnce(Return(3));
	EXPECT_CALL(mock, gl_AttachShader(1, 3));
	EXPECT_CALL(mock, gl_GetError()).WillRepeatedly(Return(GL_NO_ERROR));

	// The compile call
    EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));

	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar"));
}

TEST(ProgramTest, useUnlinkedProgramWillLinkIt) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
	// The compile call
    EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
	EXPECT_CALL(mock, gl_UseProgram(1));
	auto p = ogls::Program();
	p.addShader(ogls::FragmentShader().addSource("foo bar")).use();
}

TEST(ProgramTest, linkIsOnlyCalledWhenNeeded) {
	// If nothing has changed, glCompileShader is only called once
	{
		NiceMock<GLMock> mock;
		EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
		EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(1));
		EXPECT_CALL(mock, gl_LinkProgram(1)).Times(1);
		EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(1));
		EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
		auto shdr = ogls::Program().addShader(ogls::FragmentShader().addSource("foo bar")).link();
		shdr.link();
	}

	// but is a source is added a new link will be triggered 
	{
		NiceMock<GLMock> mock;
		EXPECT_CALL(mock, gl_CreateShader(_)).WillRepeatedly(Return(1));
		EXPECT_CALL(mock, gl_CreateProgram()).WillRepeatedly(Return(1));
		EXPECT_CALL(mock, gl_LinkProgram(1)).Times(2);
		EXPECT_CALL(mock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
		EXPECT_CALL(mock, gl_GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(1));
		auto shdr = ogls::Program().addShader(ogls::FragmentShader().addSource("foo bar")).link();
		shdr.addShader(ogls::VertexShader().addSource("foo bar")).link();
	}
}

TEST(ProgramTest, shaderExceptionHasReadableErrors) {
	NiceMock<GLMock> mock;
	EXPECT_CALL(mock, gl_CreateProgram()).WillOnce(Return(0));

	try {
		ogls::Program();
	} catch (ogls::ProgramException e) {
		ASSERT_STREQ("Failed to create program", e.what());
	}
}