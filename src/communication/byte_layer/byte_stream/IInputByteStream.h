#ifndef CROSSING_CONSOLES_I_STREAM_IN_H
#define CROSSING_CONSOLES_I_STREAM_IN_H

namespace communication {
namespace byte_layer {

/**
 * \brief Abstract object with which bytes can be received.
 */
class IInputByteStream {
 public:
  virtual ~IInputByteStream() = default;

  /**
   * \brief Indicates if data is available and can be read.
   */
  virtual bool HasInput() = 0;

  /**
   * \brief Read data.
   * \details Never blocks.
   */
  virtual size_t Read(uint8_t* receive_buffer, size_t max_length) = 0;

  /**
   * \brief Read a string without blocking.
   */
  std::string ReadStringWithoutBlocking(size_t max_length = 1024) {
    char* receive_buffer = new char[max_length + 1];
    auto read_count = Read(reinterpret_cast<uint8_t*>(receive_buffer), max_length);
    receive_buffer[read_count] = '\0';
    std::string received(receive_buffer);
    delete[] receive_buffer;
    return received;
  }
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_I_STREAM_IN_H
