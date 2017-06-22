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

#include "mbed.h"
#include "GPSProviderImplBase.h"
#include "GPSProvider.h"

bool
GPSProvider::setPowerMode(PowerMode_t power)
{
    return impl->setPowerMode(power);
}

void
GPSProvider::reset(void)
{
    impl->reset();
}

void
GPSProvider::start(void)
{
    impl->start();
}

void
GPSProvider::stop(void)
{
    impl->stop();
}

void
GPSProvider::process(void)
{
    impl->process();
}

bool
GPSProvider::haveDeviceInfo(void) const
{
    return impl->haveDeviceInfo();
}

const char *
GPSProvider::getDeviceInfo(void) const
{
    return impl->getDeviceInfo();
}

uint32_t
GPSProvider::ioctl(uint32_t command, void *arg)
{
    return impl->ioctl(command, arg);
}

bool
GPSProvider::locationAvailable(void) const
{
    return impl->locationAvailable();
}

const GPSProvider::LocationUpdateParams_t *
GPSProvider::getLastLocation(void) const
{
    return impl->getLastLocation();
}

void
GPSProvider::onLocationUpdate(LocationUpdateCallback_t callback)
{
    impl->onLocationUpdate(callback);
}

void
GPSProvider::lpmGetImmediateLocation(void)
{
    impl->lpmGetImmediateLocation();
}

// [ST-GNSS] - Geofencing API
bool
GPSProvider::isGeofencingSupported(void) const
{
    return impl->isGeofencingSupported();
}
// [ST-GNSS] - Geofencing API
gps_provider_error_t
GPSProvider::configGeofences(GPSGeofence *geofences[])
{ 
  return impl->configGeofences(geofences);
}
// [ST-GNSS] - Geofencing API
gps_provider_error_t
GPSProvider::geofenceReq(void)
{
  return impl->geofenceReq();
}
// [ST-GNSS] - Geofencing API
void
GPSProvider::onGeofencesTrigger(GeofencesTriggerCallback_t callback)
{
    impl->onGeofencesTrigger(callback);
}