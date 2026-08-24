/**
 * @file SubframeFactory.cpp
 * @brief Реализация фабрики сабфреймов.
 */

#include "model/SubframeFactory.h"

#include <stdexcept>

#include "model/subframes/EmptySubframe.h"
#include "model/subframes/NpbchSubframe.h"
#include "model/subframes/NpdcchSubframe.h"
#include "model/subframes/NpdschSubframe.h"
#include "model/subframes/NpssSubframe.h"
#include "model/subframes/NsssSubframe.h"

namespace nbiot {

std::unique_ptr<Subframe> SubframeFactory::create(const SubframeType type,
                                                  const SubframeContext context) {
    switch (type) {
        case SubframeType::Empty:
            return std::make_unique<EmptySubframe>(context);
        case SubframeType::Npbch:
            return std::make_unique<NpbchSubframe>(context);
        case SubframeType::Npss:
            return std::make_unique<NpssSubframe>(context);
        case SubframeType::Nsss:
            return std::make_unique<NsssSubframe>(context);
        case SubframeType::Npdsch:
            return std::make_unique<NpdschSubframe>(context);
        case SubframeType::Npdcch:
            return std::make_unique<NpdcchSubframe>(context);
    }

    throw std::invalid_argument("Неизвестный тип сабфрейма");
}

}  // namespace nbiot
