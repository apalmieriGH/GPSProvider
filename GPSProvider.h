/* mbed Microcontroller Library
 * Copyright (c) 2006-2014 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __GPS_PROVIDER_H__
#define __GPS_PROVIDER_H__

// [ST-GNSS] - Geofencing API
class GPSGeofence; /* forward declaration */

class GPSProviderImplBase; /* forward declaration */
extern GPSProviderImplBase *createGPSProviderInstance(void);

//
// Here's a snippet showing how this API can be used. The handler gets invoked
// from thread context--i.e. from the main application.
//
//      void handleGPSData(const LocationUpdateParams_t *newLocation) {
//          ...
//      }
//
//      GPSProvider gps;
//
//      gps.setPowerMode(LOW_POWER);
//      gps.onLocationUpdate(handleGPSData);
//
//      gps.reset();
//      gps.start();
//
//      while (true) {
//          /* purely optional */
//          static bool printedDeviceInfo = false;
//          if (!printedDeviceInfo && gps.haveDeviceInfo()) {
//              printf("%s", gps.getDeviceInfo());
//              printedDeviceInfo = true;
//          }
//
//          /* Main message processing activity; location callbacks are called
//           * as a result of message processing. */
//          gps.process();
//          /* sleep(); */
//
//          if (/* at some point in the future */) {
//              break;
//          }
//      }
//      gps.stop();
//

class GPSProvider {
public:
    /** Power mode selection */
    enum PowerMode_t {
         POWER_FULL, /**< full-power mode typically results in high-accuracy location data updated at 1Hz. */
         POWER_LOW,  /**< low-power mode involves longer periods of hibernation in between location updates. */
    };

    /**
     * GPS time starts from UTC 01/06/1980(MM/DD/YYYY) and includes 2 fields:
     * week and tow. For a given GPS time, week is the passed week number since
     * the GPS start time, and tow(time of week) is the passed seconds since the
     * last Sunday. For example, a given UTC time [10/30/3014
     * 01:10:00](MM/DD/YYYY HH:MM:SS) can be converted into GPS time [1816,
     * 4200](week, seconds).
     *
     * GPS time can be used as a quite accurate time once position is fixed.
     */
    struct GPSTime_t {
        uint16_t gps_week;
        uint32_t tow;       /* time of week (in millisecond) */
    };

    typedef float LocationType_t;
    typedef float Altitude_t;
    struct LocationUpdateParams_t {
        uint32_t       version; /* Layout-version for the following structure;
                                 * this is to accommodate changes over time. */
        bool           valid;   /* Does this update contain a valid location. */
        LocationType_t lat;
        LocationType_t lon;
        Altitude_t     altitude;

        unsigned       numGPSSVs; /* num GPS Satellites */
        unsigned       numGLOSVs; /* num GLONASS Satellites */

        GPSTime_t      gpsTime; /* gps time */
        uint64_t       utcTime; /* UTC time in millisecond */
    };

    // [ST-GNSS] - Geofencing API
    typedef float GeofenceCircleDistance_t;
    struct GeofencesTriggerParams_t {
      uint8_t bitmap;
      GPSGeofence *triggeringGeofence;
      GeofenceCircleDistance_t distance;
      int currentStatus;
    };

public:
    /**
     * Set the operating mode for power. Typically this allows the user to
     * choose between various low-power modes. Entering low-power modes often
     * results in a trade-off between accuracy and power consumption.
     *
     * The new mode takes effect upon calling start().
     *
     * @param power The new power mode.
     * @return      true if the update was successful.
     */
    bool setPowerMode(PowerMode_t power);

    /**
     * HW reset to get location chip into hibernation mode, but in readiness for
     * starting operation. The GPS controller emerges from hibernation when
     * start() is called.
     *
     * The typical initialization sequence is:
     *   reset();
     *   start(); // and thereafter receive location-update callbacks.
     */
    void reset(void);

    /**
     * Start the GPS operation, taking into account the operation mode set
     * previously. Following this call, the user may expect to receive location
     * notifications in interrupt context if a handler has previously been set.
     *
     * @note: calling start repeatedly doesn't hurt.
     */
    void start(void);

    /**
     * Stop active operation of the GPS; and put it to hibernation.
     *
     * @note: You don't need to call reset() afterwards to enter hibernation.
     * @note: Calling stop() repeatedly doesn't hurt.
     */
    void stop(void);

    /**
     * Process location data from chip and update location and satellite
     * information. This API is supposed to be called repeatedly from the
     * application in thread mode to process incoming messages as they are
     * received from the GPS controller. Arriving data is first appended to
     * something like a circular buffer by interrupts, and then parsed as
     * messages in thread mode.
     *
     * The application typically enters a loop calling process() after
     * initializing the GPS controller with start(). process() returns
     * immediately if there is no work to be done, but it must get invoked
     * frequently in order to keep pace with arriving data.
     *
     * Mbed's sleep() may be usefully thrown into the application's process()
     * loop to save power--sleep() has the effect of putting the processor to
     * sleep while waiting for an event (such as an interrupt). As always, some
     * care must be taken in employing sleep(), because there is a small
     * synchronization window where an interrupt may arrive and pend data which
     * doesn't get processed (as illustrated below).
     *
     *  while (true) {
     *      process();
     *        <--  interrupt arrives now and appends new data
     *      sleep(); // but then we go to sleep without processing it.
     *  }
     *
     * There is a way around it: if sleep() boils down to the use of ARM's WFE
     * instruction (as opposed to WFI), then its safe from the above-mentioned
     * synchronization window.
     */
    void process(void);

    /**
     * @return  true if the initialization process has received enough
     *     information to determine the hardware's version/device-info.
     */
    bool haveDeviceInfo(void) const;

    /**
     * Fetch the device information string. This is expected to contain the
     * version number or any other device identifier.
     */
    const char *getDeviceInfo(void) const;

    /**
     * This is a wildcard API for sending controller specific commands. Use of
     * this API will make programs non-portable, but then there may arise a
     * genuine need to access special functionality.
     *
     * @param  command   A controller specific command.
     * @param  arg       Argument to the command; interpreted according to the command.
     * @return           any return from the command.
     */
    uint32_t ioctl(uint32_t command, void *arg);

    // [ST-GNSS] - Geofencing API
    gps_provider_error_t configGeofences(GPSGeofence *geofences[]);
    // [ST-GNSS] - Geofencing API
    gps_provider_error_t geofenceReq(void);
    
    /**
     * @return  true if we've obtained at least one valid location since last
     *     calling start().
     *
     * @Note: This is cleared after reset().
     */
    bool locationAvailable(void) const;

    /**
     * @return  the last valid location if there is any; else NULL.
     */
    const LocationUpdateParams_t *getLastLocation(void) const;

    /**
     * Type declaration for a callback to be invoked from interrupt context upon
     * receiving new location data.
     *
     * @Note: Please note that the handler gets invoked from interrupt context.
     * Users *should not* do any long running or blocking operations in the
     * handler.
     */
    typedef void (* LocationUpdateCallback_t)(const LocationUpdateParams_t *params);

    /**
     * Setup the locationUpdate callback.
     *
     * @Note: Please note that the handler gets invoked from interrupt context.
     * Users *should not* do any long running or blocking operations in the
     * handler.
     */
    void onLocationUpdate(LocationUpdateCallback_t callback);

    /**
     * In low-power operation, the GPS controller may be expected to hibernate
     * for extended periods and location updates may be infrequent. It should
     * then be possible for an application to demand location data when needed.
     *
     * This calls results in a locationCallback if there is a useful location to
     * report.
     */
    void lpmGetImmediateLocation(void);

    // [ST-GNSS] - Geofencing API
    bool isGeofencingSupported(void) const;
    // [ST-GNSS] - Geofencing API
    typedef void (* GeofencesTriggerCallback_t)(const GeofencesTriggerParams_t *params);
    // [ST-GNSS] - Geofencing API
    void onGeofencesTrigger(GeofencesTriggerCallback_t callback);
    
public:
    /**
     * Default constructor.
     */
    GPSProvider() : impl(createGPSProviderInstance()) {
        /* empty */
    }

    virtual ~GPSProvider() {
        stop();
    }

private:
    /**
     * We use 'composition' to combine a driver-implementation object to the
     * GPSProvider interface. The implementation object will come to life
     * through the createGPSProviderInstance(), which must be defined by the
     * driver library. The mechanics of the implementation are to be hidden
     * behind the abstract interface provided by GPSProvider.
     */
    GPSProviderImplBase *const impl;

    /* disallow copy constructor and assignment operators */
private:
    GPSProvider(const GPSProvider&);
    GPSProvider & operator= (const GPSProvider&);
};

#endif /*__GPS_PROVIDER_H__*/
