// ***************************************************************************
// The Ambient object
// ***************************************************************************


struct Ambient {
    int Pin_LedR;
    int Pin_LedG;
    int Pin_LedB;
    int program;

    Ambient(int Pin_LedR, int Pin_LedG, int Pin_LedB)
    :   Pin_LedR(Pin_LedR),
        Pin_LedG(Pin_LedG),
        Pin_LedB(Pin_LedB)
    {
        pinMode(Pin_LedR, OUTPUT);
        pinMode(Pin_LedG, OUTPUT);
        pinMode(Pin_LedB, OUTPUT);
        clear();
    }

    void clear() {
        analogWrite(Pin_LedR, 0);
        analogWrite(Pin_LedG, 0);
        analogWrite(Pin_LedB, 0);
    }

    void begin() {
        begin_0();
    }


    void begin_0() {
        Serial.println("ambient 0");
        program = 0;
        analogWrite(Pin_LedR, 0);
        analogWrite(Pin_LedG, 0);
        analogWrite(Pin_LedB, 0);
    }


    void begin_1() {
        Serial.println("ambient 1");
        program = 1;
        analogWrite(Pin_LedR, 1023);
        analogWrite(Pin_LedG, 1023);
        analogWrite(Pin_LedB, 1023);
    }


    void begin_2() {
        Serial.println("ambient 2");
        program = 2;
        clear();
        update_2(0, true);
    }

    void update_2(int value, bool up) {
        const int inc = 16;
        const int next = 15;

        if (program != 2) return;

        analogWrite(Pin_LedR, value);

        if (up) {
            if (value == 1024) {
                q.in(next, [=] {update_2(value, false);});
            } else {
                q.in(next, [=] {update_2(value + inc, true);});
            }
        } else {
            if (value == 0) {
                q.in(next, [=] {update_2(value, true);});
            } else {
                q.in(next, [=] {update_2(value - inc*2, false);});
            }

        }
    }

    void begin_3() {
        Serial.println("ambient 3");
        program = 3;
        clear();
        update_3(0);
    }

    void update_3(int col) {
        const int next = 1000;

        if (program != 3) return;

        if (col == 0) {
            analogWrite(Pin_LedR, 1023);
            analogWrite(Pin_LedG, 0);
            analogWrite(Pin_LedB, 0);
            q.in(next, [=] {update_3(1);});
        } else if (col == 1) {
            analogWrite(Pin_LedG, 1023);
            analogWrite(Pin_LedR, 0);
            analogWrite(Pin_LedB, 0);
            q.in(next, [=] {update_3(2);});
        } else {
            analogWrite(Pin_LedB, 1023);
            analogWrite(Pin_LedR, 0);
            analogWrite(Pin_LedG, 0);
            q.in(next, [=] {update_3(0);});
        }
    }

    void begin_4() {
        Serial.println("ambient 4");
        program = 4;
        clear();
        update_4(true);
    }

    void update_4(bool b) {
        if (program != 4) return;
        int value = b ? 1023 : 0;
        analogWrite(Pin_LedR, value);
        analogWrite(Pin_LedG, value);
        analogWrite(Pin_LedB, value);
        q.in(1000, [=] {update_4(not b);});
    }


    void begin_5() {
        Serial.println("ambient 5");
        program = 5;
        clear();
        update_5(0);
    }

    void update_5(int value) {
        if (program != 5) return;
        ++value;
        if (value == 1024) value = 0;
        analogWrite(Pin_LedR, value);
        analogWrite(Pin_LedG, value);
        analogWrite(Pin_LedB, value);
        q.in(10, [=] {update_5(value);});
    }





};
