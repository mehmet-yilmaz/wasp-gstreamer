#ifndef _WEBSOCKET_APPLICATION_HPP_
#define _WEBSOCKET_APPLICATION_HPP_

#include <shared.hpp>
#include <web/server/websocket/websocket.connection.hpp>

namespace Wasp
{

    class WSApplication
    {
    private:
        oatpp::String m_name;
        std::unordered_map<std::string, std::shared_ptr<WSConnection>> m_connections;
        std::mutex m_connectionByIdLock;

    public:
        WSApplication(const oatpp::String &name) : m_name(name){};

        /**
         * Add peer to the room.
         * @param peer
         */
        void addConnection(const std::shared_ptr<WSConnection> &peer)
        {
            std::lock_guard<std::mutex> guard(m_connectionByIdLock);
            m_connections[peer->uuid()] = peer;
        };

        /**
         * Remove Connection from the room.
         * @param connectionUUId
         */
        void removeConnectionById(const std::string &connectionUUId)
        {
            std::lock_guard<std::mutex> guard(m_connectionByIdLock);
            m_connections.erase(connectionUUId);
        };

        /**
         * Send message to all peers in the room.
         * @param message
         */
        void sendMessage(const oatpp::String &message)
        {
            std::lock_guard<std::mutex> guard(m_connectionByIdLock);
            for (auto &pair : m_connections)
            {
                pair.second->sendMessage(message);
            }
        };
    };
};

#endif // !_WEBSOCKET_APPLICATION_HPP_