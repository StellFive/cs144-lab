#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return input_closed_;
}

void Writer::push( string data )
{
  if (is_closed())
    return;
  
  size_t push_len = std::min(available_capacity(), data.size());

  for (size_t i = 0 ; i < push_len; i++)
    buffer_.push_back(data[i]);

  writer_pushed_ += push_len;
}

void Writer::close()
{
  if (is_closed())
    return;

  input_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buffer_.size() ? capacity_ - buffer_.size() : 0;
}

uint64_t Writer::bytes_pushed() const
{
  return writer_pushed_;
}

bool Reader::is_finished() const
{
  return  input_closed_ and writer_pushed_ == reader_poped_;
}

uint64_t Reader::bytes_popped() const
{
  return reader_poped_;
}

string_view Reader::peek() const
{
  if (buffer_.empty())
    return std::string_view {};

  return std::string_view{&buffer_.front(), 1};
}

void Reader::pop( uint64_t len )
{
  size_t pop_len = min(len, buffer_.size());

  reader_poped_ += pop_len;
  if (pop_len == buffer_.size())
  {
      buffer_.clear();
      return;
  }

  while (pop_len--)
    buffer_.pop_front();
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_.size();
}
