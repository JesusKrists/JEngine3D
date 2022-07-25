#pragma once

#include "JEngine3D/Core/Application.hpp"

namespace JE {

    class PeriodicTimer
    {
        friend class Application;

        static constexpr auto MILISECONDS = 1000;

    public:
        PeriodicTimer(const PeriodicTimer& other)                    = default;
        PeriodicTimer(PeriodicTimer&& other)                         = default;
        auto operator=(const PeriodicTimer& other) -> PeriodicTimer& = default;
        auto operator=(PeriodicTimer&& other) -> PeriodicTimer&      = default;

        explicit PeriodicTimer(uint32_t frequencyMs) : m_FrequencyMs(frequencyMs) { JE_APP.RegisterPeriodicTimer(*this); }
        ~PeriodicTimer() { JE_APP.UnregisterPeriodicTimer(*this); }

        template<typename T>
        inline auto Execute(T func) -> bool
        {
            if (CurrentTimerMillis() >= m_FrequencyMs) {
                func();
                m_CurrentTimer = 0;
                return true;
            }

            return false;
        }

        [[nodiscard]] inline auto CurrentTimerMillis() const -> double { return m_CurrentTimer * MILISECONDS; }

    private:
        inline void UpdateTimer() { m_CurrentTimer += JE_APP.DeltaTime(); }

        uint32_t m_FrequencyMs  = 0;
        double   m_CurrentTimer = 0;
    };

}// namespace JE