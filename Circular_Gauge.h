#include <Adafruit_SSD1306.h>

#ifndef _Circular_Gauge_H_
#define _Circular_Gauge_H_

class Circular_Gauge {
    public:
        Circular_Gauge(int min, int max);
        Circular_Gauge(int min, int max, bool includeRenderTime);

        Adafruit_SSD1306* getDisplay();

        void begin();
        void drawGaugeData(float value);
};

#endif

