/**
 ******************************************************************************
 * @file    GPSGeofence.h
 * @author  AST/CL
 * @version V1.1.0
 * @date    Jun, 2017
 * @brief   .
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#ifndef __GPS_GEOFENCE_H__
#define __GPS_GEOFENCE_H__

class GPSProvider;

class GPSGeofence {
public:
    enum {
        GEOFENCE_TRANSITION_DWELL   = 1,
        GEOFENCE_TRANSITION_ENTER   = 2,
        GEOFENCE_TRANSITION_EXIT    = 4,
    };
    
    static const int NEVER_EXPIRE = -1;
    
    static const int GEOFENCE_ID_MAX_SIZE = 32;
    
    /** 
     * Construct a GPSGeofence instance. 
     */ 
    GPSGeofence() :
        _expirationDuration(-1),
        _notificationResponsiveness(0),
        _transitionTypes(0) {
        
        memset(_geofenceId, 0, GEOFENCE_ID_MAX_SIZE);
    }
    
    virtual void setGeofenceRegion(GPSProvider::LocationType_t lat,
                                   GPSProvider::LocationType_t lon,
                                   GPSProvider::LocationType_t radius) {
        _lat = lat;
        _lon = lon;
        _radius = radius;
    }
    
    virtual void setExpirationDuration (long durationMillis) = 0;
    
    virtual void setNotificationResponsiveness (int notificationResponsivenessMs) {
        _notificationResponsiveness = notificationResponsivenessMs;
    }
    
    virtual void setGeofenceId(const char *geofenceId) {
        if(strlen(geofenceId) > GEOFENCE_ID_MAX_SIZE) {
            return;
        }
        memcpy(_geofenceId, geofenceId, strlen(geofenceId));
    }
    
    virtual void setTransitionTypes(int transitionTypes) {
        _transitionTypes = transitionTypes;
    }

protected:
    char                        _geofenceId[GEOFENCE_ID_MAX_SIZE];
    GPSProvider::LocationType_t _lat;
    GPSProvider::LocationType_t _lon;
    GPSProvider::LocationType_t _radius;
    long                        _expirationDuration;
    int                         _notificationResponsiveness;
    int                         _transitionTypes;

private:
    /* disallow copy constructor and assignment operators */
    GPSGeofence(const GPSGeofence&);
    GPSGeofence & operator= (const GPSGeofence&);
};

#endif /* __GPS_GEOFENCE_H__ */
