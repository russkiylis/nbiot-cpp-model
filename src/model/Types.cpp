/**
 * @file Types.cpp
 * @brief Реализация вспомогательных функций общих типов модели.
 */

#include "model/Types.h"

namespace nbiot {

std::string_view toString(const SubframeType type) noexcept {
    switch (type) {
        case SubframeType::Empty:
            return "Empty";
        case SubframeType::Npbch:
            return "NPBCH";
        case SubframeType::Npss:
            return "NPSS";
        case SubframeType::Nsss:
            return "NSSS";
        case SubframeType::Npdsch:
            return "NPDSCH";
        case SubframeType::Npdcch:
            return "NPDCCH";
    }

    return "Unknown";
}

bool supportsNrs(const SubframeType type) noexcept {
    return type != SubframeType::Npss && type != SubframeType::Nsss;
}

}  // namespace nbiot
