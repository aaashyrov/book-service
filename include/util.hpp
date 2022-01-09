//
// Created by alisher on 1/9/22.
//
#pragma once
#include <MongoDB.hpp>
#include <json/json.h>

template <typename T>
bsoncxx::document::value MessageToBsonDocumentValue(T const& message) {
    std::string json;
    json.reserve(BUFFER_SIZE);
    auto status = google::protobuf::util::MessageToJsonString(message, &json);
    if (not status.ok()) { throw std::runtime_error("Deserialize error"); }
    return bsoncxx::from_json(json);
}

template <typename T>
T BsonDocumentValueToMessage(bsoncxx::document::value const& value) {
    Result result;
    result.ok = false;

    std::string json;
    json.reserve(BUFFER_SIZE);
    json = bsoncxx::to_json(value);
    google::protobuf::util::JsonParseOptions parseOptions;
    parseOptions.ignore_unknown_fields = true;
    T message;
    auto status = google::protobuf::util::JsonStringToMessage(json, &message, parseOptions);

    if (not status.ok()) { throw std::runtime_error("Parse error"); }

    return message;
}

std::string generateId(const std::string& name) noexcept;

std::string to_string(google::protobuf::Message const& message) noexcept;

Json::Value to_json(google::protobuf::Message const& message) noexcept;