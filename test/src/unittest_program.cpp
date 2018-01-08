#include "glmock.hpp"
#include "gtest/gtest.h"

#include "program.hpp"
#include "shader.hpp"

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AnyNumber;
using ::testing::_;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::NiceMock;

class ShaderMock : public ogls::Shader {
public:
	ShaderMock() {
		ON_CALL(*this, type()).WillByDefault(Return(GL_FRAGMENT_SHADER));
		ON_CALL(*this, id()).WillByDefault(Return(1));
		ON_CALL(*this, isCompiled()).WillByDefault(Return(true));
		ON_CALL(*this, compile()).WillByDefault(ReturnRef(*this));
		ON_CALL(*this, addSource(_)).WillByDefault(ReturnRef(*this));
	}
	MOCK_METHOD0(type, GLenum());
	MOCK_METHOD0(id, GLint());
	MOCK_METHOD0(isCompiled, bool());
	MOCK_METHOD0(compile, ShaderMock&());
	MOCK_METHOD1(addSource, ShaderMock&(std::string));
};

class ProgramTest : public ::testing::Test {
public:
	NiceMock<ShaderMock> m_shaderMock;
	NiceMock<GLMock> m_glMock;

protected:
	void SetUp() {
		ON_CALL(m_glMock, gl_CreateProgram()).WillByDefault(Return(1));
		ON_CALL(m_glMock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillByDefault(SetArgPointee<2>(1));
	}
};

TEST_F(ProgramTest, createBasicProgram)
{
	EXPECT_CALL(m_glMock, gl_CreateProgram()).WillOnce(Return(1));
	ogls::Program();
}

TEST_F(ProgramTest, failCreateBasicProgram)
{
	EXPECT_CALL(m_glMock, gl_CreateProgram()).WillOnce(Return(0));
	ASSERT_THROW(ogls::Program(), ogls::ProgramException);
}

TEST_F(ProgramTest, addShader) {
	EXPECT_CALL(m_glMock, gl_CreateProgram()).WillOnce(Return(4));
	EXPECT_CALL(m_shaderMock, id()).WillOnce(Return(3));
	EXPECT_CALL(m_glMock, gl_AttachShader(4, 3));
	ogls::Program().addShader(m_shaderMock);
}

TEST_F(ProgramTest, addShaderWithGLFailures) {
	EXPECT_CALL(m_glMock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION)).WillOnce(Return(2));

	for(int i = 0; i < 3; i++)
		EXPECT_THROW(ogls::Program().addShader(m_shaderMock), ogls::ProgramException);
}

TEST_F(ProgramTest, addUncompiledShaderWillCompileIt) {
	EXPECT_CALL(m_shaderMock, isCompiled()).WillOnce(Return(false));
	EXPECT_CALL(m_shaderMock, compile());
	ogls::Program().addShader(m_shaderMock);
}

TEST_F(ProgramTest, addTwoShadersOfSameTypeWillFail) {
	NiceMock<ShaderMock> otherShader;
	ogls::Program p;
	p.addShader(otherShader);
	ASSERT_THROW(p.addShader(m_shaderMock), ogls::ProgramException);
}

TEST_F(ProgramTest, linkBasicProgram) {
	EXPECT_CALL(m_glMock, gl_LinkProgram(1));
	ogls::Program().addShader(m_shaderMock).link();
}

TEST_F(ProgramTest, aProgramWithNoShadersShouldNotBeLinked) {
	EXPECT_CALL(m_glMock, gl_LinkProgram(_)).Times(0);
	ogls::Program().link();
}

TEST_F(ProgramTest, failingToLinkThrowsException) {
	EXPECT_CALL(m_glMock, gl_GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(0));
	ASSERT_THROW(ogls::Program().addShader(m_shaderMock).link(), ogls::ProgramException);
}

TEST_F(ProgramTest, useProgram) {
	EXPECT_CALL(m_glMock, gl_UseProgram(1));
	ogls::Program().addShader(m_shaderMock).link().use();
}

TEST_F(ProgramTest, useUnlinkedProgramWillLinkIt) {
	EXPECT_CALL(m_glMock, gl_LinkProgram(1));
	ogls::Program().addShader(m_shaderMock).use();
}

TEST_F(ProgramTest, useProgramWithErrorWillThrowException) {
	auto p = ogls::Program().addShader(m_shaderMock).link();
	EXPECT_CALL(m_glMock, gl_GetError()).WillOnce(Return(GL_INVALID_VALUE)).WillOnce(Return(GL_INVALID_OPERATION)).WillOnce(Return(2));
	for(int i = 0; i < 3; i++)
		EXPECT_THROW(p.use(), ogls::ProgramException);
}

TEST_F(ProgramTest, linkIsOnlyCalledWhenNeeded) {

	EXPECT_CALL(m_glMock, gl_LinkProgram(1)).Times(1).RetiresOnSaturation();
	auto prog1 = ogls::Program().addShader(m_shaderMock).link();
	prog1.link();

	// but if a source is added a new link will be triggered 
	NiceMock<ShaderMock> shdr;
	EXPECT_CALL(m_glMock, gl_LinkProgram(_)).Times(2);
	EXPECT_CALL(shdr, type()).WillOnce(Return(GL_VERTEX_SHADER));
	auto prog2 = ogls::Program().addShader(m_shaderMock).link();
	prog2.addShader(shdr).link();

}

TEST_F(ProgramTest, setFloatUniform) {
	EXPECT_CALL(m_glMock, gl_GetUniformLocation(1, "baz")).WillRepeatedly(Return(1));
	EXPECT_CALL(m_glMock, gl_Uniform1f(1, 1.0f));
	EXPECT_CALL(m_glMock, gl_Uniform2f(1, 1.0f, 0.9f));
	EXPECT_CALL(m_glMock, gl_Uniform3f(1, 1.0f, 0.9f, 0.8f));
	ogls::Program().setUniform<float>("baz", {1.0f});
	ogls::Program().setUniform<float>("baz", {1.0f, 0.9f});
	ogls::Program().setUniform<float>("baz", {1.0f, 0.9f, 0.8f});
}

TEST_F(ProgramTest, shaderExceptionHasReadableErrors) {
	EXPECT_CALL(m_glMock, gl_CreateProgram()).WillOnce(Return(0));

	try {
		ogls::Program();
	} catch (ogls::ProgramException e) {
		ASSERT_STREQ("Failed to create program", e.what());
	}
}

TEST_F(ProgramTest, possibleToUseStreamOperator) {
	NiceMock<ShaderMock> otherShader;

	// One vertex shader wih id 4
	EXPECT_CALL(otherShader, type()).WillRepeatedly(Return(GL_VERTEX_SHADER));
	EXPECT_CALL(otherShader, id()).WillOnce(Return(4));

	// One fragment shader with id 3
	EXPECT_CALL(m_shaderMock, id()).WillOnce(Return(3));

	// Program id is 1, attach each shader
	EXPECT_CALL(m_glMock, gl_AttachShader(1, 3));
	EXPECT_CALL(m_glMock, gl_AttachShader(1, 4));

	ogls::Program p; 
	p << otherShader << m_shaderMock;
}