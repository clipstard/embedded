
/* Include guard */
#ifndef SENSOR_PUBLISHER_TASK
#define SENSOR_PUBLISHER_TASK

/* Standard C library for basic mathematical operations */
#include <math.h>
#include <stdio.h>
#include <utils/taskmanager/taskmanager.hpp>
#include <hardware/drivers/dcmotor.hpp>
#include <utils/serial/serialmonitor.hpp>
#include <signal/controllers/motorcontroller.hpp>
#include <signal/systemmodels/systemmodels.hpp>

#include <hardware/encoders/encoderinterfaces.hpp>
#include <mbed.h>
 
namespace examples
{

    namespace sensors{

       /**
        * @brief CEncoderPublisher class is subclass of utils::task::CTask, a class to publish periodically the encoder values. 
        * 
        */
        class SensorPublisher:public utils::task::CTask
        {
            public:
                /* Constructor */
                SensorPublisher(uint32_t            f_period
                            ,Serial&            f_serial);
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);
                uint32_t hcsr04_read ();
            private:
                
                /* Run method */
                void _run();
                void delay (uint32_t us);
                void initTimer();

                /** @brief Active flag  */
                bool                m_isActive;
                /** @brief Serial communication obj.  */
                Serial&             m_serial;
                uint32_t local_time, sensor_time;
                uint32_t distance;
                TIM_HandleTypeDef htim1;
        };
    }; // namespace sensors
}; // namespace examples


#endif