#ifndef MANAK_MONITOR_ENTRY_HPP_INCLUDED
#define MANAK_MONITOR_ENTRY_HPP_INCLUDED

#include <string>

namespace manak
{

struct TestEntry
{
  TestEntry(const std::string& filename,
            size_t line_no,
            const std::string& msg = "")
  : filename(filename),
    line_no(line_no),
    msg(msg)  {}

  const std::string& Filename() const
  {
    return filename;
  }

  const size_t& LineNo() const
  {
    return line_no;
  }

  virtual const std::string& Msg() const
  {
    return msg;
  }

  std::string filename;
  size_t line_no;
  std::string msg;
};

struct TrueTestEntry : public TestEntry
{
  TrueTestEntry(const std::string& filename, size_t line_no)
    : TestEntry(filename, line_no) {}
};

struct MsgEntry : public TestEntry
{
  MsgEntry(const std::string& filename,
           size_t line_no,
           const std::string& msg)
    : TestEntry(filename, line_no, msg) {}
};

}

#endif // MANAK_MONITOR_ENTRY_HPP_INCLUDED
