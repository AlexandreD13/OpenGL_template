#include "GLProgramPipeline.h"

#include <utility>

GLProgramPipeline::GLProgramPipeline(std::string name) : m_Name(std::move(name)) {
	glCreateProgramPipelines(1, &m_Pipeline);
}

GLProgramPipeline::~GLProgramPipeline() {
	glDeleteProgramPipelines(1, &m_Pipeline);
}

void GLProgramPipeline::useProgramStage(GLProgram *p, GLenum programType) const {
	if (p != nullptr) {
        glUseProgramStages(m_Pipeline, programType, p->getId());
    }
}

bool GLProgramPipeline::link() {
	GLint param = GL_FALSE;
	glValidateProgramPipeline(m_Pipeline);
	glGetProgramPipelineiv(m_Pipeline, GL_VALIDATE_STATUS, &param);

	if(param == GL_FALSE) {
		printInfoLog();
		throw std::logic_error(string("Pipeline ") + m_Name + string(" : \n") + info_text );
	}

    return param == GL_TRUE;
}

void GLProgramPipeline::bind() const {
	glBindProgramPipeline(m_Pipeline);
}

void GLProgramPipeline::release() {
	glBindProgramPipeline(0);
}

void GLProgramPipeline::printInfoLog() {
	int infologLength = 0;
	char *infoLog;

	glGetProgramPipelineiv (m_Pipeline, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0) {
		infoLog = (char *)malloc(infologLength);
		glGetProgramPipelineInfoLog(m_Pipeline, infologLength, nullptr, infoLog);
		string df(infoLog);
		info_text += df ;
		free(infoLog);
	}
}

GLuint GLProgramPipeline::getId() const {
	return m_Pipeline;
}