#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>

#include <cctype>
#include <cstring>

// Detect debugger as described in https://stackoverflow.com/a/24969863
bool runningUnderDebugger() {
  char buf[4096];

  const int status_fd = ::open("/proc/self/status", O_RDONLY);
  if (status_fd == -1)
    return false;

  const ssize_t num_read = ::read(status_fd, buf, sizeof(buf) - 1);
  if (num_read <= 0)
    return false;

  buf[num_read] = '\0';
  constexpr char tracerPidString[] = "TracerPid:";
  const auto tracer_pid_ptr = ::strstr(buf, tracerPidString);
  if (!tracer_pid_ptr)
    return false;

  for (const char *characterPtr = tracer_pid_ptr + sizeof(tracerPidString) - 1; characterPtr <= buf + num_read;
       ++characterPtr) {
    if (::isspace(*characterPtr))
      continue;
    else
      return ::isdigit(*characterPtr) != 0 && *characterPtr != '0';
  }

  return false;
}
#endif
