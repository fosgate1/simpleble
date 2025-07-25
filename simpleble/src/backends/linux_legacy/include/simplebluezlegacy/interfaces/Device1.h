#pragma once

#include <simpledbuslegacy/advanced/Interface.h>
#include <simpledbuslegacy/advanced/InterfaceRegistry.h>

#include "kvn/kvn_safe_callback.hpp"

#include <string>

#include "simplebluezlegacy/Types.h"

namespace SimpleBluezLegacy {

class Device1 : public SimpleDBusLegacy::Interface {
  public:
    Device1(std::shared_ptr<SimpleDBusLegacy::Connection> conn, std::string path);
    virtual ~Device1();

    // ----- METHODS -----
    void Connect();
    void Disconnect();
    void Pair();
    void CancelPairing();

    // ----- PROPERTIES -----
    int16_t RSSI();
    int16_t TxPower();
    uint16_t Appearance();  // On Bluez 5.53, this always returns 0.
    std::string Address();
    std::string AddressType();
    std::string Alias();
    std::string Name();
    std::vector<std::string> UUIDs();
    std::map<uint16_t, ByteArray> ManufacturerData(bool refresh = true);
    std::map<std::string, ByteArray> ServiceData(bool refresh = true);
    bool Paired(bool refresh = true);
    bool Connected(bool refresh = true);
    bool ServicesResolved(bool refresh = true);

    // ----- CALLBACKS -----
    kvn::safe_callback<void()> OnServicesResolved;
    kvn::safe_callback<void()> OnDisconnected;

  protected:
    void property_changed(std::string option_name) override;

    int16_t _rssi = INT16_MIN;
    int16_t _tx_power = INT16_MIN;
    std::string _name;
    std::string _alias;
    std::string _address;
    std::string _address_type;
    bool _connected;
    bool _services_resolved;
    std::map<uint16_t, ByteArray> _manufacturer_data;
    std::map<std::string, ByteArray> _service_data;

  private:
    static const SimpleDBusLegacy::AutoRegisterInterface<Device1> registry;
};

}  // namespace SimpleBluezLegacy
