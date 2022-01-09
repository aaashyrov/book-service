//
// Created by alisher on 1/9/22.
//
#include <util.hpp>

std::string generateId(const std::string& name) noexcept {
    auto nameHash = std::hash<std::string>()(name);
    auto id = std::hash<unsigned>()(nameHash);
    return std::to_string(id);
}

std::string to_string(google::protobuf::Message const& message) noexcept {
    std::string json;
    google::protobuf::util::MessageToJsonString(message, &json);
    return json;
}

Json::Value to_json(google::protobuf::Message const& message) noexcept {
    std::string raw = to_string(message);
    JSONCPP_STRING err;
    Json::Value value;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(raw.c_str(), raw.c_str() + raw.size(), &value, &err)) {
        std::cout << "error " << err.c_str() << std::endl;
    }

    return value;
}