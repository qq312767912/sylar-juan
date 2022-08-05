#pragma once
#include<string>
#include<stdint.h>
#include<memory>
#include<list>
#include<stringstream>
#include<fstream>

namespace sylar{

// 日志级别
class LogLevel{
public:  
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

// 日志事件
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time;
    std::string m_content;
    uint32_t m_elapse = 0; // 程序启动到现在的毫秒数
};

// 日志器
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    Logger(const std::string& name = "root"){};
    void log(LogLevel::Level level,LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender); 
    LogLevel::Level getLevel() const{return m_level};
    void setLevel(LogLevel::Level level) {m_level = level};

private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders; // Appender集合，输出地集合
};

// 日志输出地
// 基类
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}
    virtual void log(LogLevel::Level level,LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val){m_formatter = val};
    LogFormatter::ptr getFormatter() const{return m_formatter;}

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formater;
};

// 日志格式器
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    std::string format(LogEvent::ptr event);
private:
    class FormatItem{
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        virtual ~FormatItem();
        virtual void format(std::ostream& os,LogEvent::ptr event)=0;
    };
    void init();

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;

};

// 输出到控制台
class StdoutLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<StedoutLogAppender> ptr;
    void log(LogLevel::Level level,LogEvent::ptr event) override;
};

// 输出到文件
class FileLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<StedoutLogAppender> ptr;
    void log(LogLevel::Level level,LogEvent::ptr event) override;
    FileLogAppender(const std::string& filename);
    // 重新打开文件，文件打开成功返回true
    void reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};


}

