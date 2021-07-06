class NECDecoder {
  public:
    NECDecoder() {
      nec_ok = 0;
      nec_state = 0;
    }
    
    void initialize(void(*func)()) {
      lis = func;
      TCCR1A = 0;
      TCCR1B = 0;
      TCNT1  = 0;
      TIMSK1 = 1;
      attachInterrupt(0, lis, CHANGE);
    }

    void remote_read() {
      unsigned int timer_value;
      if(nec_state != 0){
        timer_value = TCNT1;                         // Store Timer1 value
        TCNT1 = 0;                                   // Reset Timer1
      }
      switch(nec_state){
       case 0 :                                      // Start receiving IR data (we're at the beginning of 9ms pulse)
        TCNT1  = 0;                                  // Reset Timer1
        TCCR1B = 2;                                  // Enable Timer1 module with 1/8 prescaler ( 2 ticks every 1 us)
        nec_state = 1;                               // Next state: end of 9ms pulse (start of 4.5ms space)
        i = 0;
        return;
       case 1 :                                      // End of 9ms pulse
        if((timer_value > 19000) || (timer_value < 17000)){         // Invalid interval ==> stop decoding and reset
          nec_state = 0;                             // Reset decoding process
          TCCR1B = 0;                                // Disable Timer1 module
        }
        else
          nec_state = 2;                             // Next state: end of 4.5ms space (start of 562µs pulse)
        return;
       case 2 :                                      // End of 4.5ms space
        if((timer_value > 10000) || (timer_value < 8000)){
          nec_state = 0;                             // Reset decoding process
          TCCR1B = 0;                                // Disable Timer1 module
        }
        else
          nec_state = 3;                             // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
        return;
       case 3 :                                      // End of 562µs pulse
        if((timer_value > 1400) || (timer_value < 800)){           // Invalid interval ==> stop decoding and reset
          TCCR1B = 0;                                // Disable Timer1 module
          nec_state = 0;                             // Reset decoding process
        }
        else
          nec_state = 4;                             // Next state: end of 562µs or 1687µs space
        return;
       case 4 :                                      // End of 562µs or 1687µs space
        if((timer_value > 3600) || (timer_value < 800)){           // Time interval invalid ==> stop decoding
          TCCR1B = 0;                                // Disable Timer1 module
          nec_state = 0;                             // Reset decoding process
          return;
        }
        if( timer_value > 2000)                      // If space width > 1ms (short space)
          bitSet(nec_code, (31 - i));                // Write 1 to bit (31 - i)
        else                                         // If space width < 1ms (long space)
          bitClear(nec_code, (31 - i));              // Write 0 to bit (31 - i)
        i++;
        if(i > 31){                                  // If all bits are received
          nec_ok = 1;                                // Decoding process OK
          detachInterrupt(0);                        // Disable external interrupt (INT0)
          return;
        }
        nec_state = 3;                               // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
      }
    }

    void isr() {
      nec_state = 0;
      TCCR1B = 0;
    }

    void refresh() {
      if(nec_ok) {
        nec_ok = 0;
        nec_state = 0;
        TCCR1B = 0;
        address = nec_code >> 16;
        command = nec_code >> 8;
        inv_command = nec_code;
        attachInterrupt(0, lis, CHANGE);
      }
    }

    unsigned int getAddress() {
      return address;
    }

    byte getCommand() {
      return command;
    }

    void print() {
      Serial.print("Address: ");
      Serial.print(address, HEX);
      
      Serial.print("\tCommand BIN: ");
      for(int i=7; i>=0; i--)
        Serial.print(bitRead(command, i));
      Serial.println(); 
    }
    
  private:
    boolean nec_ok;
    byte i, nec_state, command, inv_command;
    unsigned int address;
    unsigned long nec_code;
    void* lis;
};

NECDecoder rec;
void listen() {rec.remote_read();}
ISR(TIMER1_OVF_vect) {rec.isr();}

void setup() {
  Serial.begin(9600);
  rec.initialize(listen);
  
}

void loop() {
  rec.refresh();

  rec.print();

}
