/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "AirflowNetworkSurface.hpp"
#include "AirflowNetworkSurface_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
//#include "SurfaceAirflowLeakage.hpp"
//#include "SurfaceAirflowLeakage_Impl.hpp"
//#include "ExternalNode.hpp"
//#include "ExternalNode_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
//#include "AirflowNetworkOccupantVentilationControl.hpp"
//#include "AirflowNetworkOccupantVentilationControl_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkSurface_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSurface::iddObjectType());
  }

  AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkSurface::iddObjectType());
  }

  AirflowNetworkSurface_Impl::AirflowNetworkSurface_Impl(const AirflowNetworkSurface_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirflowNetworkSurface_Impl::iddObjectType() const {
    return AirflowNetworkSurface::iddObjectType();
  }

std::vector<ScheduleTypeKey> AirflowNetworkSurface_Impl::getScheduleTypeKeys(const Schedule& schedule) const
{
  // TODO: Check schedule display names.
  std::vector<ScheduleTypeKey> result;
  UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
  UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
  if (std::find(b,e,OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName) != e)
  {
    result.push_back(ScheduleTypeKey("AirflowNetworkSurface","Ventilation Control Zone Temperature Setpoint"));
  }
  if (std::find(b,e,OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName) != e)
  {
    result.push_back(ScheduleTypeKey("AirflowNetworkSurface","Venting Availability"));
  }
  return result;
}

boost::optional<PlanarSurface> AirflowNetworkSurface_Impl::surface() const {
  return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_AirflowNetworkSurfaceFields::SurfaceName);
  //boost::optional<PlanarSurface> value = optionalSurface();
  //if (!value) {
  //  LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
  //}
  //return value.get();
}

//SurfaceAirflowLeakage AirflowNetworkSurface_Impl::leakageComponent() const {
//  boost::optional<SurfaceAirflowLeakage> value = optionalLeakageComponent();
//  if (!value) {
//    LOG_AND_THROW(briefDescription() << " does not have an Leakage Component attached.");
//  }
//  return value.get();
//}

//boost::optional<ExternalNode> AirflowNetworkSurface_Impl::externalNode() const {
//  return getObject<ModelObject>().getModelObjectTarget<ExternalNode>(OS_AirflowNetworkSurfaceFields::ExternalNodeName);
//}

double AirflowNetworkSurface_Impl::windowDoorOpeningFactorOrCrackFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isWindowDoorOpeningFactorOrCrackFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor);
}

std::string AirflowNetworkSurface_Impl::ventilationControlMode() const {
  boost::optional<std::string> value = getString(OS_AirflowNetworkSurfaceFields::VentilationControlMode,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isVentilationControlModeDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::VentilationControlMode);
}

boost::optional<Schedule> AirflowNetworkSurface_Impl::ventilationControlZoneTemperatureSetpointSchedule() const {
  return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName);
}

double AirflowNetworkSurface_Impl::minimumVentingOpenFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isMinimumVentingOpenFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor);
}

double AirflowNetworkSurface_Impl::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
}

double AirflowNetworkSurface_Impl::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
}

double AirflowNetworkSurface_Impl::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
}

double AirflowNetworkSurface_Impl::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
  boost::optional<double> value = getDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor,true);
  OS_ASSERT(value);
  return value.get();
}

bool AirflowNetworkSurface_Impl::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
  return isEmpty(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
}

boost::optional<Schedule> AirflowNetworkSurface_Impl::ventingAvailabilitySchedule() const {
  return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName);
}

//boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkSurface_Impl::occupantVentilationControl() const {
//  return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkOccupantVentilationControl>(OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName);
//}

bool AirflowNetworkSurface_Impl::setSurface(const PlanarSurface& surfAndSubSurf) {
  bool result = setPointer(OS_AirflowNetworkSurfaceFields::SurfaceName, surfAndSubSurf.handle());
  return result;
}

//bool AirflowNetworkSurface_Impl::setLeakageComponent(const SurfaceAirflowLeakage& surfaceAirflowLeakage) {
//  bool result = setPointer(OS_AirflowNetworkSurfaceFields::LeakageComponentName, surfaceAirflowLeakage.handle());
//  return result;
//}

//bool AirflowNetworkSurface_Impl::setExternalNode(const boost::optional<ExternalNode>& externalNode) {
//  bool result(false);
//  if (externalNode) {
//    result = setPointer(OS_AirflowNetworkSurfaceFields::ExternalNodeName, externalNode.get().handle());
//  }
//  else {
//    resetExternalNode();
//    result = true;
//  }
//  return result;
//}

//void AirflowNetworkSurface_Impl::resetExternalNode() {
//  bool result = setString(OS_AirflowNetworkSurfaceFields::ExternalNodeName, "");
//  OS_ASSERT(result);
//}

bool AirflowNetworkSurface_Impl::setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor, windowDoorOpeningFactorOrCrackFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetWindowDoorOpeningFactorOrCrackFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::Window_DoorOpeningFactororCrackFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setVentilationControlMode(std::string ventilationControlMode) {
  bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlMode, ventilationControlMode);
  return result;
}

void AirflowNetworkSurface_Impl::resetVentilationControlMode() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlMode, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
  bool result = setSchedule(OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName,
                            "AirflowNetworkSurface",
                            "Ventilation Control Zone Temperature Setpoint",
                            schedule);
  return result;
}

void AirflowNetworkSurface_Impl::resetVentilationControlZoneTemperatureSetpointSchedule() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor, minimumVentingOpenFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetMinimumVentingOpenFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::MinimumVentingOpenFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
  bool result = setDouble(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
  return result;
}

void AirflowNetworkSurface_Impl::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, "");
  OS_ASSERT(result);
}

bool AirflowNetworkSurface_Impl::setVentingAvailabilitySchedule(Schedule& schedule) {
  bool result = setSchedule(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName,
                            "AirflowNetworkSurface",
                            "Venting Availability",
                            schedule);
  return result;
}

void AirflowNetworkSurface_Impl::resetVentingAvailabilitySchedule() {
  bool result = setString(OS_AirflowNetworkSurfaceFields::VentingAvailabilityScheduleName, "");
  OS_ASSERT(result);
}

//bool AirflowNetworkSurface_Impl::setOccupantVentilationControl(const boost::optional<AirflowNetworkOccupantVentilationControl>& airflowNetworkOccupantVentilationControl) {
//  bool result(false);
//  if (airflowNetworkOccupantVentilationControl) {
//    result = setPointer(OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName, airflowNetworkOccupantVentilationControl.get().handle());
//  }
//  else {
//    resetOccupantVentilationControl();
//    result = true;
//  }
//  return result;
//}

//void AirflowNetworkSurface_Impl::resetOccupantVentilationControl() {
//  bool result = setString(OS_AirflowNetworkSurfaceFields::OccupantVentilationControlName, "");
//  OS_ASSERT(result);
//}

//boost::optional<PlanarSurface> AirflowNetworkSurface_Impl::optionalSurface() const {
//  return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_AirflowNetworkSurfaceFields::SurfaceName);
//}

//boost::optional<SurfaceAirflowLeakage> AirflowNetworkSurface_Impl::optionalLeakageComponent() const {
//  return getObject<ModelObject>().getModelObjectTarget<SurfaceAirflowLeakage>(OS_AirflowNetworkSurfaceFields::LeakageComponentName);
//}

} // detail

AirflowNetworkSurface::AirflowNetworkSurface(const Model& model, const PlanarSurface &surface)
  : ModelObject(AirflowNetworkSurface::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkSurface_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirflowNetworkSurfaceFields::SurfaceName
  //     OS_AirflowNetworkSurfaceFields::LeakageComponentName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surface);
  OS_ASSERT(ok);
  // ok = setLeakageComponent();
  OS_ASSERT(ok);
}

IddObjectType AirflowNetworkSurface::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirflowNetworkSurface);
}

std::vector<std::string> AirflowNetworkSurface::ventilationControlModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirflowNetworkSurfaceFields::VentilationControlMode);
}

boost::optional<PlanarSurface> AirflowNetworkSurface::surface() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->surface();
}

//SurfaceAirflowLeakage AirflowNetworkSurface::leakageComponent() const {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->leakageComponent();
//}

//boost::optional<ExternalNode> AirflowNetworkSurface::externalNode() const {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->externalNode();
//}

double AirflowNetworkSurface::windowDoorOpeningFactorOrCrackFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->windowDoorOpeningFactorOrCrackFactor();
}

bool AirflowNetworkSurface::isWindowDoorOpeningFactorOrCrackFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isWindowDoorOpeningFactorOrCrackFactorDefaulted();
}

std::string AirflowNetworkSurface::ventilationControlMode() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->ventilationControlMode();
}

bool AirflowNetworkSurface::isVentilationControlModeDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isVentilationControlModeDefaulted();
}

boost::optional<Schedule> AirflowNetworkSurface::ventilationControlZoneTemperatureSetpointSchedule() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->ventilationControlZoneTemperatureSetpointSchedule();
}

double AirflowNetworkSurface::minimumVentingOpenFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->minimumVentingOpenFactor();
}

bool AirflowNetworkSurface::isMinimumVentingOpenFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isMinimumVentingOpenFactorDefaulted();
}

double AirflowNetworkSurface::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
}

bool AirflowNetworkSurface::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
}

double AirflowNetworkSurface::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
}

bool AirflowNetworkSurface::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
}

double AirflowNetworkSurface::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
}

bool AirflowNetworkSurface::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
}

double AirflowNetworkSurface::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
}

bool AirflowNetworkSurface::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
}

boost::optional<Schedule> AirflowNetworkSurface::ventingAvailabilitySchedule() const {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->ventingAvailabilitySchedule();
}

//boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkSurface::occupantVentilationControl() const {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->occupantVentilationControl();
//}

bool AirflowNetworkSurface::setSurface(const PlanarSurface& surfAndSubSurf) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setSurface(surfAndSubSurf);
}

//bool AirflowNetworkSurface::setLeakageComponent(const SurfaceAirflowLeakage& surfaceAirflowLeakage) {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->setLeakageComponent(surfaceAirflowLeakage);
//}

//bool AirflowNetworkSurface::setExternalNode(const ExternalNode& externalNode) {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->setExternalNode(externalNode);
//}

//void AirflowNetworkSurface::resetExternalNode() {
//  getImpl<detail::AirflowNetworkSurface_Impl>()->resetExternalNode();
//}

bool AirflowNetworkSurface::setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setWindowDoorOpeningFactorOrCrackFactor(windowDoorOpeningFactorOrCrackFactor);
}

void AirflowNetworkSurface::resetWindowDoorOpeningFactorOrCrackFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetWindowDoorOpeningFactorOrCrackFactor();
}

bool AirflowNetworkSurface::setVentilationControlMode(std::string ventilationControlMode) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentilationControlMode(ventilationControlMode);
}

void AirflowNetworkSurface::resetVentilationControlMode() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentilationControlMode();
}

bool AirflowNetworkSurface::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentilationControlZoneTemperatureSetpointSchedule(schedule);
}

void AirflowNetworkSurface::resetVentilationControlZoneTemperatureSetpointSchedule() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentilationControlZoneTemperatureSetpointSchedule();
}

bool AirflowNetworkSurface::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setMinimumVentingOpenFactor(minimumVentingOpenFactor);
}

void AirflowNetworkSurface::resetMinimumVentingOpenFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetMinimumVentingOpenFactor();
}

bool AirflowNetworkSurface::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
}

void AirflowNetworkSurface::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
}

bool AirflowNetworkSurface::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
}

void AirflowNetworkSurface::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
}

bool AirflowNetworkSurface::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
}

void AirflowNetworkSurface::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
}

bool AirflowNetworkSurface::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
}

void AirflowNetworkSurface::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
}

bool AirflowNetworkSurface::setVentingAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirflowNetworkSurface_Impl>()->setVentingAvailabilitySchedule(schedule);
}

void AirflowNetworkSurface::resetVentingAvailabilitySchedule() {
  getImpl<detail::AirflowNetworkSurface_Impl>()->resetVentingAvailabilitySchedule();
}

//bool AirflowNetworkSurface::setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl) {
//  return getImpl<detail::AirflowNetworkSurface_Impl>()->setOccupantVentilationControl(airflowNetworkOccupantVentilationControl);
//}

//void AirflowNetworkSurface::resetOccupantVentilationControl() {
//  getImpl<detail::AirflowNetworkSurface_Impl>()->resetOccupantVentilationControl();
//}

/// @cond
AirflowNetworkSurface::AirflowNetworkSurface(std::shared_ptr<detail::AirflowNetworkSurface_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
