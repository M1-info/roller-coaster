#include "utils/Shader.h"

Shader::Shader(const std::string &filename) : m_Filename(filename), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

ShaderProgramSource Shader::ParseShader()
{
    std::string filepath;
#if VISUAL_STUDIO
    filepath = "src/assets/shaders/" + m_Filename;
#elif MINGW
    filepath = "app\\src\\assets\\shaders\\" + m_Filename;
#endif

    std::stringstream ss[2];
    for (int i = 0; i < 2; i++)
    {
        std::ifstream stream(filepath + (i == 0 ? ".vert" : ".frag"));
        if (!stream.is_open())
        {
            std::cerr << "Failed to open file: " << m_Filename << std::endl;
            exit(1);
        }

        std::string line;
        while (getline(stream, line))
        {
            ss[i] << line << "\n";
        }

        stream.close();
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteProgram(fs);

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int shaderId = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)_malloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    GLint location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetUniform1f(const std::string &name, float value)
{
    GLint location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetUniform3f(const std::string &name, float v0, float v1, float v2)
{
    GLint location = GetUniformLocation(name);
    glUniform3f(location, v0, v1, v2);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLint location = GetUniformLocation(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 matrix)
{
    GLint location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

GLint Shader::GetUniformLocation(const std::string &name) const
{
    // we store the uniform location in a cache
    // so we don't have to query the GPU every time
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    // if the uniform doesn't exist, we get -1
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    // we store the uniform location in the cache before returning it
    m_UniformLocationCache[name] = location;
    return location;
}

bool Shader::HasUniform(const std::string &name)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    return location != -1;
}
