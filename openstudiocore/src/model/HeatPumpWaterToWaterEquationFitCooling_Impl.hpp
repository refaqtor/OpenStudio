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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class HeatPumpWaterToWaterEquationFitHeating;

namespace detail {

  /** HeatPumpWaterToWaterEquationFitCooling_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpWaterToWaterEquationFitCooling.*/
  class MODEL_API HeatPumpWaterToWaterEquationFitCooling_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~HeatPumpWaterToWaterEquationFitCooling_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> referenceLoadSideFlowRate() const;

    double ratedLoadSideFlowRate() const;

    bool isReferenceLoadSideFlowRateAutosized() const;

    boost::optional<double> referenceSourceSideFlowRate() const;

    double ratedSourceSideFlowRate() const;

    bool isReferenceSourceSideFlowRateAutosized() const;

    boost::optional<double> ratedCoolingCapacity() const;

    bool isRatedCoolingCapacityAutosized() const;

    boost::optional<double> ratedCoolingPowerConsumption() const;

    bool isRatedCoolingPowerConsumptionAutosized() const;

    double coolingCapacityCoefficient1() const;

    double coolingCapacityCoefficient2() const;

    double coolingCapacityCoefficient3() const;

    double coolingCapacityCoefficient4() const;

    double coolingCapacityCoefficient5() const;

    double coolingCompressorPowerCoefficient1() const;

    double coolingCompressorPowerCoefficient2() const;

    double coolingCompressorPowerCoefficient3() const;

    double coolingCompressorPowerCoefficient4() const;

    double coolingCompressorPowerCoefficient5() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    boost::optional<HeatPumpWaterToWaterEquationFitHeating> companionHeatingHeatPump() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

    void autosizeReferenceLoadSideFlowRate();

    bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

    void autosizeReferenceSourceSideFlowRate();

    bool setRatedCoolingCapacity(double ratedCoolingCapacity);

    void autosizeRatedCoolingCapacity();

    bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

    void autosizeRatedCoolingPowerConsumption();

    void setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1);

    void setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2);

    void setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3);

    void setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4);

    void setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5);

    void setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1);

    void setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2);

    void setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3);

    void setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4);

    void setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5);

    void setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    void setSizingFactor(double sizingFactor);

    bool setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP);

    //@}
    /** @name Other */
    //@{

    virtual unsigned supplyInletPort();

    virtual unsigned supplyOutletPort();

    virtual unsigned demandInletPort();

    virtual unsigned demandOutletPort();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP

