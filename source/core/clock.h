//
// Created by Osprey on 5/20/2025.
//

#pragma once

#ifndef TIME_H
#define TIME_H

#endif //TIME_H

namespace ProtoCADCore {
    class Clock {
    private:
        static double m_t;
        static double m_dt;

        static double m_lastTime;
        static double m_frameTime;

        static double m_accumulator;

        static double m_deltaTime;

    public:
        static double GetDeltaTime();
        static void Tick();
    };
}