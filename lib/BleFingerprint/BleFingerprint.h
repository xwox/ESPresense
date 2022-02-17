#ifndef _BLEFINGERPRINT_
#define _BLEFINGERPRINT_

#include "GUI.h"
#include <ArduinoJson.h>
#include <NimBLEAdvertisedDevice.h>
#include <NimBLEBeacon.h>
#include <NimBLEDevice.h>
#include <NimBLEEddystoneTLM.h>
#include <NimBLEEddystoneURL.h>
#include <SoftFilters.h>

#define NO_RSSI -32768

#define ID_TYPE_TX_POW short(1)
#define ID_TYPE_AD short(6)
#define ID_TYPE_SD short(7)
#define ID_TYPE_MD short(8)
#define ID_TYPE_MISC_APPLE short(9)
#define ID_TYPE_NAME short(10)
#define ID_TYPE_MISC short(11)
#define ID_TYPE_MSFT short(12)
#define ID_TYPE_SONOS short(13)
#define ID_TYPE_MITHERM short(14)
#define ID_TYPE_MIFIT short(15)
#define ID_TYPE_EXPOSURE short(16)
#define ID_TYPE_ITAG short(17)
#define ID_TYPE_TRACKR short(18)
#define ID_TYPE_TILE short(19)
#define ID_TYPE_MEATER short(20)
#define ID_TYPE_APPLE_NEARBY short(35)
#define ID_TYPE_QUERY_MODEL short(40)
#define ID_TYPE_QUERY_NAME short(50)
#define ID_TYPE_EBEACON short(97)
#define ID_TYPE_ABEACON short(98)
#define ID_TYPE_IBEACON short(99)
#define ID_TYPE_RM_ASST short(100)

class BleFingerprintCollection;

class BleFingerprint
{

public:
    BleFingerprint(const BleFingerprintCollection *parent, NimBLEAdvertisedDevice *advertisedDevice, float fcmin,
                   float beta, float dcutoff);

    bool seen(BLEAdvertisedDevice *advertisedDevice);
    bool report(JsonDocument *doc);
    bool query();

    const String getId()
    {
        if (!id.isEmpty() && idType > 10) return id;
        if (macPublic) return getMac();
        if (!id.isEmpty()) return id;
        return getMac();
    }

    void setId(const String& newId, short int newIdType);

    String getMac() const;
    int get1mRssi();
    String getDiscriminator() { return disc; }

    float getDistance() { return output.value.position; }
    int getRssi() { return rssi; }
    int getNewestRssi() { return newest; }

    void setInitial(int rssi, float distance);

    NimBLEAddress getAddress() { return address; }
    long getAge() { return millis() - lastSeenMillis; };
    bool getIgnore() const { return ignore; };
    bool getAllowQuery() { return allowQuery; };
    bool getRmAsst() { return rmAsst; };
    int getSeenCount()
    {
        auto sc = seenCount;
        seenCount = 0;
        return sc;
    }

private:
    void fingerprint(NimBLEAdvertisedDevice *advertisedDevice);
    bool shouldHide(const String& s);

    const BleFingerprintCollection *_parent{};
    bool hasValue = false, close = false, reported = false, macPublic = false, ignore = false, allowQuery = false, didQuery = false, rmAsst = false, hidden = false, connectable = false;
    NimBLEAddress address;
    String id, name, disc;
    short int idType = 0;
    int rssi = -100, calRssi = NO_RSSI, mdRssi = NO_RSSI, asRssi = NO_RSSI, newest = NO_RSSI, recent = NO_RSSI, oldest = NO_RSSI;
    int qryAttempts = 0, seenCount = 1, qryDelayMillis = 0;
    float raw = 0, lastReported = 0, temp = 0, humidity = 0;
    unsigned long firstSeenMillis, lastSeenMillis = 0, lastReportedMillis = 0, lastQryMillis = 0;
    uint16_t mv = 0;

    Reading<Differential<float>> output;

    OneEuroFilter<float, unsigned long> oneEuro;
    DifferentialFilter<float, unsigned long> diffFilter;

    bool filter();
};
#endif
