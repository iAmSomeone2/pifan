#pragma once

#include <cstdint>
#include <string>

namespace Pifan::Hardware
{
    enum class ResistorState
    {
        Off = 0,
        PullDown = 1,
        PullUp = 2
    };

    enum class PinMode
    {
        Input = 0,
        Output = 1,
        Alt0 = 4,
        Alt1 = 5,
        Alt2 = 6,
        Alt3 = 7,
        Alt4 = 3,
        Alt5 = 2
    };

    enum class PinState
    {
        Low = 0,
        High = 1
    };

    class Pin
    {
    protected:
        uint8_t target{}; /**<Numeric value of Pi to target when making API calls*/
        uint8_t pinNumber{}; /**<Broadcom index of this Pin*/
        std::string pinLabel;

        PinMode mode; /**<Current mode of pin*/
        PinState state; /**<Current state of pin*/
    public:
        /**
         * \brief Constructs a new Pin instance.
         * \param index Broadcom pin number
         * \param target Numeric value of Pi to target when making API calls
         */
        Pin(const uint8_t& index, const uint8_t& target);

        /**
         * \brief Constructs a new Pin instance
         * \param index Broadcom pin number
         * \param target Numeric value of Pi to target when making API calls
         * \param label label to give to the pin.
         */
        Pin(const uint8_t& index, const uint8_t& target, std::string label);

        /**
         * \brief Changes state of internal pin resistor.
         * \param resistorState state to set internal resistor to.
         */
        void setPullUpDown(const ResistorState& resistorState) const;

        /**
         * \brief Sets a new pin mode.
         *
         * \details
         * Changes PinMode of instance to match 'newMode'. The changes are sent to the daemon and
         * updated in the instance on success.
         *
         * \param newMode New PinMode to use with this Pin
         */
        void setMode(const PinMode& newMode);

        /**
         * \brief Gets the current pin mode from the instance.
         *
         * \details
         * This method does not check the mode with the PiGPIO daemon. The object keeps track
         * of mode changes as they happen to reduce complexity and API calls.
         *
         * \return Current PinMode of pin instance.
         */
        [[nodiscard]] const PinMode& getMode() const;

        /**
         * \brief Writes a new state to the pin and updates the instance.
         * \param newState state to assign to pin.
         */
        void write(const PinState& newState);

        /**
         * \brief Reads the current state of pin and updates the instance.
         * \return Current state of pin.
         */
        [[nodiscard]] const PinState& read();
    };
}