/*
 * Message.h
 *
 *    Created on: Mar 8, 2021
 *            Author: ubuntu
 */

#pragma once

#include <string>
#include <cstring>
#include <memory>
#include <netinet/in.h>

namespace transport
{

class Message : public std::enable_shared_from_this<Message>
{

public:
    struct Header
    {
        std::uint64_t bodyLength;

        inline bool decode()
        {
            bodyLength = ntohl(bodyLength);
            return bodyLength > 0;
        }

        inline bool encode()
        {
            if (bodyLength <= 0)
            {
                return false;
            }
            bodyLength = htonl(bodyLength);
            return true;
        }
    };

    static const size_t SIZE_OF_HEADER = sizeof(Header);

    Header header;

    std::uint8_t* body;

public:

    Message()
    {
        header.bodyLength = 0;
        body = nullptr;
        release = true;
        offset = 0;
    }

    virtual ~Message()
    {
        if (release && body != nullptr)
        {
            delete[] body;
        }
    }

    static std::shared_ptr<Message> Generate(size_t size = 0)
    {
        auto msg = std::make_shared<Message>();
        if (size > 0)
        {
            msg->body = new std::uint8_t[size];
        }
        msg->header.bodyLength = size;
        return msg;
    }

    static std::shared_ptr<Message> Generate(std::shared_ptr<Header> header, std::uint8_t* data = nullptr)
    {
        auto msg = std::make_shared<Message>();

        msg->header = *header;

        if (data == nullptr)
        {
            if (header->bodyLength > 0)
            {
                msg->body = new std::uint8_t[header->bodyLength];
            }
        }
        else
        {
            msg->body = data;
            msg->release = false;
        }

        return msg;
    }

    void Reset()
    {
        offset = 0;
    }

    void IgnoreBytes(size_t size)
    {
        offset += size;
    }

    std::uint8_t* GetDataPointerWithOffset()
    {
        return body + offset;
    }

    template <typename T>
    bool AppendData(T data)
    {
        auto&& size = sizeof(T);
        if (offset + size <= header.bodyLength)
        {
            memcpy(body + offset, &data, size);
            offset += size;
            return true;
        }
        return false;
    }

    bool AppendData(std::string data)
    {
        auto size = data.size();
        if (offset + size + sizeof(size_t) <= header.bodyLength)
        {
            memcpy(body + offset, &size, sizeof(size_t));
            offset += sizeof(size_t);
            memcpy(body + offset, data.c_str(), size);
            offset += data.size();
            return true;
        }
        return false;
    }

    bool AppendData(std::uint8_t* data, size_t size)
    {
        if (offset + size <= header.bodyLength)
        {
            memcpy(body + offset, data, size);
            offset += size;
            return true;
        }
        return false;
    }

    template <typename T>
    void NextData(T& data)
    {
        memcpy(&data, body + offset, sizeof(T));
        offset += sizeof(T);
    }

    void NextData(std::string& data)
    {
        size_t size;
        memcpy(&size, body + offset, sizeof(size_t));
        offset += sizeof(size_t);
        auto temp = new char[size + 1];
        memcpy(temp, body + offset, size);
        offset += size;
        temp[size] = '\0';

        std::string str(temp);
        delete[] temp;
        data = str;
    }

    void NextData(std::uint8_t* data, size_t size)
    {
        memcpy(data, body + offset, size);
        offset += size;
    }

private:
    size_t offset;
    bool release;
};

}
