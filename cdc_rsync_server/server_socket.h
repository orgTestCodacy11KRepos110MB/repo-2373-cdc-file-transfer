/*
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CDC_RSYNC_SERVER_SERVER_SOCKET_H_
#define CDC_RSYNC_SERVER_SERVER_SOCKET_H_

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "cdc_rsync/base/socket.h"

namespace cdc_ft {

class ServerSocket : public Socket {
 public:
  ServerSocket();
  ~ServerSocket();

  // Creates a TCP socket and binds to |port|.
  absl::Status Bind(int port);

  // Starts listening for connections on the first available port in the range
  // [|first_port|, |first_port|]. Returns the port.
  // Adds tag Tag::kAddressInUse if all addresses are in use.
  absl::StatusOr<int> BindToAnyPort(int first_port, int last_port);

  // Starts listening for connections. Must be in bound state.
  absl::Status StartListening();

  // Stops listening for connections. No-op if already stopped/never started.
  void StopListening();

  // Waits for a client to connect. Only supports one connection. Repeating
  // the call with an existing connection results in an error.
  absl::Status WaitForConnection();

  // Disconnects the client. No-op if not connected.
  void Disconnect();

  // Shuts down the sending end of the socket. This will interrupt any receive
  // calls on the client and shut it down.
  absl::Status ShutdownSendingEnd();

  // Socket:
  absl::Status Send(const void* buffer, size_t size) override;
  absl::Status Receive(void* buffer, size_t size, bool allow_partial_read,
                       size_t* bytes_received) override;

 private:
  std::unique_ptr<struct ServerSocketInfo> socket_info_;
};

}  // namespace cdc_ft

#endif  // CDC_RSYNC_SERVER_SERVER_SOCKET_H_
