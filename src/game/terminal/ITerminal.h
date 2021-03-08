#ifndef CROSSING_CONSOLES_I_TERMINAL_H
#define CROSSING_CONSOLES_I_TERMINAL_H

/**
 * \brief Uniform interface for the terminal input and output.
 */
class ITerminal {
 public:
  /**
   * \brief Is an new input available?
   */
  virtual bool HasInput() = 0;

  /**
   * \brief Get new input.
   */
  virtual int GetInput() = 0;

  /**
   * \brief Clear the screen and set it to the specified content.
   */
  virtual void SetScreen(const std::wstring &content) = 0;
};

#endif  // CROSSING_CONSOLES_I_TERMINAL_H