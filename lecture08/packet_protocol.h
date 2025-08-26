// packet_protocol.h
#ifndef PACKET_PROTOCOL_H
#define PACKET_PROTOCOL_H

#include <vector>
#include <string>
#include <cstdint>
#include "nlohmann/json.hpp"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

// PacketID Definition
enum PacketID {
    REQ_RESUME = 1001,
    CMD_RESUME_ACK = 1002,
    REQ_INQUIRY = 1003,
    CMD_INQUIRY = 1004,
    REQ_ANSWER = 1005,
    CMD_ANSWER_ACK = 1006
};

class JsonPacket {
protected:
    uint32_t packet_id;
    std::string json_body;
public:
    JsonPacket(uint32_t id) : packet_id(id) {}
    virtual ~JsonPacket() = default;
    static uint32_t hostToLE32(uint32_t value) {
        uint8_t bytes[4];
        bytes[0] = value & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        bytes[2] = (value >> 16) & 0xFF;
        bytes[3] = (value >> 24) & 0xFF;
        return *reinterpret_cast<uint32_t*>(bytes);
    }
    static uint32_t leToHost32(uint32_t value) {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);
        return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    }
    std::vector<uint8_t> serialize() {
        std::vector<uint8_t> packet;
        uint32_t le_id = hostToLE32(packet_id);
        uint32_t le_len = hostToLE32(static_cast<uint32_t>(json_body.length()));
        packet.insert(packet.end(),
            reinterpret_cast<uint8_t*>(&le_id),
            reinterpret_cast<uint8_t*>(&le_id) + sizeof(uint32_t));
        packet.insert(packet.end(),
            reinterpret_cast<uint8_t*>(&le_len),
            reinterpret_cast<uint8_t*>(&le_len) + sizeof(uint32_t));
        packet.insert(packet.end(), json_body.begin(), json_body.end());
        return packet;
    }
    bool deserialize(const uint8_t* data, size_t size) {
        if (size < 8) return false;
        memcpy(&packet_id, data, sizeof(uint32_t));
        packet_id = leToHost32(packet_id);
        uint32_t body_len;
        memcpy(&body_len, data + 4, sizeof(uint32_t));
        body_len = leToHost32(body_len);
        if (size < 8 + body_len) return false;
        json_body = std::string(reinterpret_cast<const char*>(data + 8), body_len);
        return true;
    }
    uint32_t getPacketID() const { return packet_id; }
    const std::string& getJsonBody() const { return json_body; }
    void setJsonBody(const std::string& body) { json_body = body; }
};

class ResumePacket : public JsonPacket {
public:
    std::string name, greeting, introduction;
    ResumePacket() : JsonPacket(REQ_RESUME) {}
    void setData(const std::string& name, const std::string& greeting, const std::string& introduction) {
        this->name = name;
        this->greeting = greeting;
        this->introduction = introduction;
        nlohmann::json j;
        j["name"] = name;
        j["greeting"] = greeting;
        j["introduction"] = introduction;
        setJsonBody(j.dump());
    }
    void parseBody() {
        auto j = nlohmann::json::parse(getJsonBody(), nullptr, false);
        name = j.value("name", "");
        greeting = j.value("greeting", "");
        introduction = j.value("introduction", "");
    }
};

class InquiryPacket : public JsonPacket {
public:
    std::string question_code;
    InquiryPacket() : JsonPacket(REQ_INQUIRY) {}
    void setData(const std::string& code) {
        question_code = code;
        nlohmann::json j;
        j["QuestionCode"] = code;
        setJsonBody(j.dump());
    }
    void parseBody() {
        auto j = nlohmann::json::parse(getJsonBody(), nullptr, false);
        question_code = j.value("QuestionCode", "");
    }
};

class AnswerPacket : public JsonPacket {
public:
    std::string question_id, answer;
    AnswerPacket() : JsonPacket(REQ_ANSWER) {}
    void setData(const std::string& q_id, const std::string& ans) {
        question_id = q_id;
        answer = ans;
        nlohmann::json j;
        j["QuestionId"] = q_id;
        j["Answer"] = ans;
        setJsonBody(j.dump());
    }
    void parseBody() {
        auto j = nlohmann::json::parse(getJsonBody(), nullptr, false);
        question_id = j.value("QuestionId", "");
        answer = j.value("Answer", "");
    }
};

class AckPacket : public JsonPacket {
public:
    uint32_t error_code;
    std::string message;
    AckPacket(uint32_t id) : JsonPacket(id), error_code(0) {}
    void setData(uint32_t err, const std::string& msg) {
        error_code = err;
        message = msg;
        nlohmann::json j;
        j["ErrorCode"] = err;
        j["Message"] = msg;
        setJsonBody(j.dump());
    }
    void parseJson() {
        auto j = nlohmann::json::parse(getJsonBody(), nullptr, false);
        error_code = j.value("ErrorCode", -1);
        message = j.value("Message", "");
    }
};

class InquiryResponsePacket : public JsonPacket {
public:
    struct Question {
        std::string question;
        std::string selection;
        std::vector<std::string> choices;
    };
    std::vector<Question> questions;
    InquiryResponsePacket() : JsonPacket(CMD_INQUIRY) {}
    void addQuestion(const std::string& q, const std::string& sel, const std::vector<std::string>& ch) {
        questions.push_back({q, sel, ch});
        updateJson();
    }
    void parseBody() {
        questions.clear();
        auto j = nlohmann::json::parse(getJsonBody(), nullptr, false);
        if (j.contains("Questions") && j["Questions"].is_array()) {
            for (const auto& item : j["Questions"]) {
                Question q;
                q.question = item.value("Question", "");
                q.selection = item.value("Selection", "");
                if (item.contains("Choices") && item["Choices"].is_array()) {
                    for (const auto& choice : item["Choices"]) {
                        q.choices.push_back(choice.get<std::string>());
                    }
                }
                questions.push_back(q);
            }
        }
    }
private:
    void updateJson() {
        nlohmann::json j;
        nlohmann::json q_array = nlohmann::json::array();
        for (const auto& q : questions) {
            nlohmann::json q_obj;
            q_obj["Question"] = q.question;
            q_obj["Selection"] = q.selection;
            q_obj["Choices"] = q.choices;
            q_array.push_back(q_obj);
        }
        j["Questions"] = q_array;
        setJsonBody(j.dump());
    }
};

#endif // PACKET_PROTOCOL_H
