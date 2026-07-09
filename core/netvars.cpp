//
// Created by sh1rm4n on 7/8/26.
//

#include "netvars.hpp"

#include "interfaces.hpp"
#include "utils.hpp"

void NetVars::Prepare() noexcept {
    for (auto clientClass = interfaces->client->GetAllClasses(); clientClass; clientClass = clientClass->GetNext()) {
        const auto recvTable = clientClass->GetRecvTable();

        if (!recvTable)
            continue;

        Dump(recvTable->m_pNetTableName, recvTable);
    }
}

void NetVars::Dump(const std::string &className, RecvTable *table, std::uint32_t offset) noexcept {
    const auto props = table->m_pProps;

    for (auto i = 0; i < table->m_nProps; i++) {
        const auto prop = &props[i];

        if (!prop)
            continue;

        if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->m_pNetTableName[0] == 'D')
            Dump(className, prop->dataTable, offset + prop->offset);

        offsets[std::format("{}->{}", className, prop->varName)] = prop->offset;
    }
}

std::uint32_t NetVars::GetOffset(const std::string &value) noexcept {
    return offsets[value];
}