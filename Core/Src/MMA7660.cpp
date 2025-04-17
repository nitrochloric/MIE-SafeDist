/*
    MMA7760.h
    Library for accelerometer_MMA7760

    Copyright (c) 2013 seeed technology inc.
    Author        :   FrankieChu
    Create Time   :   Jan 2013
    Change Log    :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "MMA7660.hpp"
#include "cmsis_os.h"
#include "stm32f7xx_ll_i2c.h"
//#include "stm32f7xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c1;
#define MMA7660TIMEOUT  500         // us
/*Function: Write a byte to the register of the MMA7660*/

void MMA7660::write(uint8_t _register, uint8_t _data) {
	HAL_I2C_Master_Transmit(&hi2c1, MMA7660_ADDR,&_register,1,160); //Sending in Blocking mode
	//HAL_Delay(10);
	HAL_I2C_Master_Transmit(&hi2c1, MMA7660_ADDR,&_data,1,160);
	//HAL_Delay(10);
}

/*Function: Read a byte from the regitster of the MMA7660*/
uint8_t MMA7660::read(uint8_t _register) {
    uint8_t data_read;
    HAL_I2C_Master_Transmit(&hi2c1, MMA7660_ADDR,&_register,1,160); //Sending in Blocking mode
	//HAL_Delay(10);
    HAL_I2C_Master_Receive(&hi2c1, MMA7660_ADDR ,&data_read, 1,160); //Receiving in Blocking mode
    //HAL_Delay(10);
    return data_read;
}

// populate lookup table based on the MMA7660 datasheet at http://www.farnell.com/datasheets/1670762.pdf
void MMA7660::initAccelTable() {
    int i;
    float val, valZ;

    for (i = 0, val = 0; i < 32; i++) {
        accLookup[i].g = val;
        val += 0.047;
    }

    for (i = 63, val = -0.047; i > 31; i--) {
        accLookup[i].g = val;
        val -= 0.047;
    }

    for (i = 0, val = 0, valZ = 90; i < 22; i++) {
        accLookup[i].xyAngle = val;
        accLookup[i].zAngle = valZ;

        val += 2.69;
        valZ -= 2.69;
    }

    for (i = 63, val = -2.69, valZ = -87.31; i > 42; i--) {
        accLookup[i].xyAngle = val;
        accLookup[i].zAngle = valZ;

        val -= 2.69;
        valZ += 2.69;
    }

    for (i = 22; i < 43; i++) {
        accLookup[i].xyAngle = 255;
        accLookup[i].zAngle = 255;
    }
}

void MMA7660::init() {
    initAccelTable();
    setMode(MMA7660_STAND_BY);
    setSampleRate(AUTO_SLEEP_32);
    setMode(MMA7660_ACTIVE);
}

void MMA7660::init(uint8_t interrupts) {
    initAccelTable();
    setMode(MMA7660_STAND_BY);
    setSampleRate(AUTO_SLEEP_32);
    write(MMA7660_INTSU, interrupts);
    setMode(MMA7660_ACTIVE);
}
void MMA7660::setMode(uint8_t mode) {
    write(MMA7660_MODE, mode);
}
void MMA7660::setSampleRate(uint8_t rate) {
    write(MMA7660_SR, rate);
}

/*Function: Get the contents of the registers in the MMA7660*/
/*          so as to calculate the acceleration.            */
uint8_t MMA7660::getXYZ(int8_t* x, int8_t* y, int8_t* z) {

    uint8_t val[3] = {0,0,0};
    HAL_I2C_Master_Transmit(&hi2c1, MMA7660_ADDR,(uint8_t*)0x00,1,180); //Sending in Blocking mode
	//HAL_Delay(10);
    HAL_I2C_Master_Receive(&hi2c1, MMA7660_ADDR,val, 3,180);
    //HAL_Delay(10);

    *x = ((int8_t)(val[0] << 2)) / 4;
    *y = ((int8_t)(val[1] << 2)) / 4;
    *z = ((int8_t)(val[2] << 2)) / 4;

    return 1;
}

uint8_t MMA7660::getAcceleration(uint32_t* ax, uint32_t* ay, uint32_t* az) {
    int8_t x, y, z;
    if (!getXYZ(&x, &y, &z)) {
        return 0;
    }
    *ax = x / 21.00;
    *ay = y / 21.00;
    *az = z / 21.00;

    return 1;
}
/*
bool MMA7660::getAcceleration(MMA7660_ACC_DATA* data) {
    unsigned char val[3];
    int count;
    bool error;

    unsigned long timer_s = micros();
    do {
        error = false;
        count = 0;

        while (Wire.available() > 0) {
            Wire.read();
        }

        Wire.requestFrom(MMA7660_ADDR, 3);
        while (Wire.available()) {
            if (count < 3) {
                val[count] = Wire.read();
                if (0x40 & val[count] == 0x40) { // alert bit is set, data is garbage and we have to start over.
                    error = true;
                    break;
                }
            }
            count++;
        }

        if (micros() - timer_s > MMA7660TIMEOUT) {
            return 0;
        }
    } while (error);

    (*data).x = accLookup[val[0]];
    (*data).y = accLookup[val[1]];
    (*data).z = accLookup[val[2]];

    return 1;
}

bool MMA7660::getAllData(MMA7660_DATA* data) {
    int count = 0;
    uint8_t val[11] = {0};

    while (Wire.available() > 0) {
        Wire.read();
    }

    Wire.requestFrom(MMA7660_ADDR, 11);
    while (Wire.available()) {
        if (count < 11) {
            val[count] = Wire.read();
        }
        count++;
    }

    data->X = val[0];
    data->Y = val[1];
    data->Z = val[2];
    data->TILT = val[3];
    data->SRST = val[4];
    data->SPCNT = val[5];
    data->INTSU = val[6];
    data->MODE = val[7];
    data->SR = val[8];
    data->PDET = val[9];
    data->PD = val[10];

    return 1;
}*/

extern "C" {
    MMA7660_Handle MMA7660_create(void) {
        return new MMA7660();
    }

    void MMA7660_destroy(MMA7660_Handle handle) {
        delete static_cast<MMA7660*>(handle);
    }

    void MMA7660_init(MMA7660_Handle handle) {
        static_cast<MMA7660*>(handle)->init();
    }

    void MMA7660_initWithInterrupts(MMA7660_Handle handle, uint8_t interrupts) {
        static_cast<MMA7660*>(handle)->init(interrupts);
    }

    void MMA7660_setMode(MMA7660_Handle handle, uint8_t mode) {
        static_cast<MMA7660*>(handle)->setMode(mode);
    }

    void MMA7660_setSampleRate(MMA7660_Handle handle, uint8_t rate) {
        static_cast<MMA7660*>(handle)->setSampleRate(rate);
    }

    uint8_t MMA7660_getXYZ(MMA7660_Handle handle, int8_t* x, int8_t* y, int8_t* z) {
        return static_cast<MMA7660*>(handle)->getXYZ(x, y, z);
    }

    uint8_t MMA7660_getAccelerationFloat(MMA7660_Handle handle, uint32_t* ax, uint32_t* ay, uint32_t* az) {
        return static_cast<MMA7660*>(handle)->getAcceleration(ax, ay, az);
    }

    // Note: Uncomment and implement these if you need them
    /*
    bool MMA7660_getAccelerationData(MMA7660_Handle handle, MMA7660_ACC_DATA* data) {
        return static_cast<MMA7660*>(handle)->getAcceleration(data);
    }

    bool MMA7660_getAllData(MMA7660_Handle handle, MMA7660_DATA* data) {
        return static_cast<MMA7660*>(handle)->getAllData(data);
    }
    */
}
