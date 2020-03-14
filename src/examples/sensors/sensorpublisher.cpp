#include <examples/sensors/sensorpublisher.hpp>

namespace examples
{

    namespace sensors{

        /** \brief CEncoderPublisher contructor
         *
         *  It initializes the member parameter, the publisher is initially deactivated.
         *
         *  @param f_period       period value
         *  @param f_encoder      reference to encoder object
         *  @param f_serial       reference to the serial object
         */
        SensorPublisher::SensorPublisher(uint32_t            f_period
                                        ,Serial&             f_serial)
            :utils::task::CTask(f_period)
            ,m_isActive(false)
            ,m_serial(f_serial)
        {
            initTimer();
        }

       
        /** \brief  Serial callback method to activate or deactivate the publisher. 
         * When the received integer value is bigger or equal to 1, then the publisher become 
         * active and send messages, otherwise is deactivated. 
         *
         * @param a                   input received string
         * @param b                   output reponse message
         * 
         */
        void SensorPublisher::serialCallback(char const * a, char * b){
            int l_isActivate=0;
            uint32_t l_res = sscanf(a,"%d",&l_isActivate);
            if(l_res==1){
                m_isActive=(l_isActivate>=1);
                sprintf(b,"ack;;");
            }else{
                sprintf(b,"sintax error;;");
            }
        }

        /** \brief It's periodically applied method to send message to other device. 
         */
        void SensorPublisher::_run()
        {
            if(!m_isActive) return;
            float f_distance = 0.0, b_distance = 0.0;
            f_distance = SensorPublisher::hcsr04_read() * .034/2;
            m_serial.printf("@USEN:%d;%.2f;;\r\n",(int)f_distance, b_distance);  
        }

        void SensorPublisher::initTimer() {
            __HAL_RCC_TIM2_CLK_ENABLE();
            htim1.Instance = TIM2;
            htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
            htim1.Init.Period = 32;
            htim1.Init.Prescaler = 1344;
            htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
            htim1.Init.RepetitionCounter = 0;
            HAL_TIM_Base_Init(&htim1); 
        }     

        void SensorPublisher::delay (uint32_t us)
        {
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            HAL_TIM_Base_Start_IT(&htim1);
            while ((__HAL_TIM_GET_COUNTER(&htim1))<us);
        }        

        uint32_t SensorPublisher::hcsr04_read (void)
        {
            local_time=0;
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  // pull the TRIG pin HIGH
            HAL_Delay(10);

            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  // pull the TRIG pin HIGH
            HAL_Delay(10);  // wait for 10 us
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  // pull the TRIG pin low

            // read the time for which the pin is high

            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))    // while the pin is high
            {
                local_time++;   // measure time for which the pin is high
                HAL_Delay(1);
            }

            return local_time;
        }           

    }; // namespace sensors
}; // namespace examples
