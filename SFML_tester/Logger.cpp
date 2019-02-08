/*

	MIT License

	Copyright (c) 2017 Nikita Kogut (MrOnlineCoder)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Description: Logger class logs given formatted string into Vanilla2DEngine.log
*/


#include "Logger.h"
#include "Logger.h"

const string Logger::m_sFileName = "engine.log";
Logger* Logger::m_pThis = NULL;
ofstream Logger::m_Logfile;
sf::Clock Logger::m_clock;

Logger::Logger()
{
 
}
Logger* Logger::GetLogger(){
    if (m_pThis == NULL){
        m_pThis = new Logger();
        m_Logfile.open(m_sFileName.c_str(), ios::out);
    }
    return m_pThis;
}

void Logger::Cleanup()
{
	if (m_pThis)
	{
		m_pThis->m_Logfile.close();
		delete m_pThis;
	}
}
 
void Logger::Log(const std::string& tag, const char * format, ...)
{
    char* sMessage = NULL;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
	stringstream msg("");
	msg << "[" << tag << " at " << m_clock.getElapsedTime().asSeconds() << "s] " << format << "\n";
    m_Logfile << msg.str();
	std::cerr << msg.str();
    va_end(args);
 
    delete [] sMessage;
}
 
void Logger::Log(const std::string& tag, std::string& sMessage)
{
	stringstream msg("");
	msg << "[" << tag << " at " << m_clock.getElapsedTime().asSeconds() << "s] " << sMessage << "\n";
	m_Logfile << msg.str();
	std::cerr << msg.str();
}
 
Logger& Logger::operator<<(const string& sMessage)
{
	stringstream msg("");
	msg << "[" << "Default" << " at " << m_clock.getElapsedTime().asSeconds() << "s] " << sMessage << "\n";
	m_Logfile << msg.str();
	std::cerr << msg.str();
    return *this;
}
