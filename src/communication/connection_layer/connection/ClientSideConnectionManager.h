#ifndef CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H

#include "ConnectionManager.h"

namespace communication {
namespace connection_layer {

/**
 * \brief `ConnectionManager` on Client Side
 */
class ClientSideConnectionManager : public ConnectionManager {
 public:
  static std::shared_ptr<ClientSideConnectionManager> CreateClientSide(
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout,
      std::shared_ptr<byte_layer::IConnectionSimulatorProvider> connection_simulator_provider =
          byte_layer::PerfectConnectionSimulatorProvider::instance);
  /**
   * \brief Check for and process new messages and timeouts.
   * \details On client side only the server connection is managed.
   */
  void HandleConnections(std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now()) override;

  /**
   * \brief Send a Message from a Client to the Server
   */
  void SendMessageToServer(const std::shared_ptr<message_layer::Message>& msg);

  /**
   * \brief Send data from a Client to the Server
   */
  void SendDataToServer(std::vector<uint8_t> data);

  /**
   * \brief Notify server that connection will be closed.
   * \details Send ResetConnection message, wait for acknowledge message before actually closing the connection.
   */
  void CloseConnectionWithServer();

  /**
   * \brief Return the underlying `SocketByteStream` or a nullptr.
   * \details Used to change the connection simulators on the fly.
   */
  byte_layer::SocketByteStream* GetSocketByteStream();

 private:
  ClientSideConnectionManager(ProtocolDefinition::timeout_t timeout);

  partner_id_t GetNextPartnerId() override;

  byte_layer::SocketByteStream* socket_byte_stream;
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
