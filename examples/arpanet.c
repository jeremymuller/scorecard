#include "deck.h"

#define FUND_FREQ 55

card_title("Arpanet");

const uint8_t tempo = 120;
float secondsPerBeat = 60.0 / tempo;
float arpRhythm = 0.25;
float gate = 1.0;
float duty = 0.4;
uint8_t currentHarmonies[9];

const uint16_t *harmonics[] = {
    (uint16_t[]){1, 2, 4, 0},
    (uint16_t[]){1, 3, 9, 0},
    (uint16_t[]){1, 5, 25, 0},

    (uint16_t[]){1, 3, 5, 15, 0},
    (uint16_t[]){1, 2, 3, 6, 0},
    (uint16_t[]){1, 2, 5, 10, 0},

    (uint16_t[]){2, 3, 4, 6, 12, 0},
    (uint16_t[]){2, 3, 6, 9, 18, 0},
    (uint16_t[]){2, 5, 10, 25, 50, 0},
    (uint16_t[]){2, 4, 5, 10, 20, 0},
    (uint16_t[]){3, 5, 15, 25, 75, 0},
    (uint16_t[]){3, 5, 9, 15, 45, 0},

    (uint16_t[]){1, 2, 5, 10, 25, 50, 0},
    (uint16_t[]){1, 3, 5, 15, 25, 75, 0},
    (uint16_t[]){1, 2, 3, 4, 6, 12, 0},
    (uint16_t[]){1, 2, 4, 5, 10, 20, 0},
    (uint16_t[]){1, 3, 5, 9, 15, 45, 0},
    (uint16_t[]){1, 2, 3, 6, 9, 18, 0},

    (uint16_t[]){2, 3, 5, 6, 10, 15, 30, 0},

    (uint16_t[]){3, 5, 9, 15, 25, 45, 75, 225, 0},
    (uint16_t[]){2, 3, 4, 6, 9, 12, 18, 36, 0},
    (uint16_t[]){1, 2, 3, 5, 6, 10, 15, 30, 0},
    (uint16_t[]){2, 4, 5, 10, 20, 25, 50, 100, 0},

    (uint16_t[]){1, 2, 3, 4, 6, 9, 12, 18, 36, 0},
    (uint16_t[]){1, 2, 4, 5, 10, 20, 25, 50, 100, 0},
    (uint16_t[]){1, 3, 5, 9, 15, 25, 45, 75, 225, 0},

    (uint16_t[]){2, 3, 5, 6, 9, 10, 15, 18, 30, 45, 90, 0},
    (uint16_t[]){2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60, 0},
    (uint16_t[]){2, 3, 5, 6, 10, 15, 25, 30, 50, 75, 150, 0},
    (uint16_t[]){1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60, 0},
    (uint16_t[]){1, 2, 3, 5, 6, 9, 10, 15, 18, 30, 45, 90, 0},
    (uint16_t[]){1, 2, 3, 5, 6, 10, 15, 25, 30, 50, 75, 150, 0},
    (uint16_t[]){2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 30, 36, 45, 60, 90, 180, 0},
    (uint16_t[]){1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 30, 36, 45, 60, 90, 180, 0},
    (uint16_t[]){2, 3, 4, 5, 6, 10, 12, 15, 20, 25, 30, 50, 60, 75, 100, 150, 300, 0},
    (uint16_t[]){2, 3, 5, 6, 9, 10, 15, 18, 25, 30, 45, 50, 75, 90, 150, 225, 450, 0},
    (uint16_t[]){1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 25, 30, 50, 60, 75, 100, 150, 300, 0},
    (uint16_t[]){1, 2, 3, 5, 6, 9, 10, 15, 18, 25, 30, 45, 50, 75, 90, 150, 225, 450, 0},
    (uint16_t[]){2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 25, 30, 36, 45, 50, 60, 75, 90, 100, 150, 180, 225, 300, 450, 900, 0},
    (uint16_t[]){1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 25, 30, 36, 45, 50, 60, 75, 90, 100, 150, 180, 225, 300, 450, 900, 0},

    (uint16_t[]){1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 24, 25, 30, 40, 50, 60, 75, 100, 120, 150, 200, 300, 600, 0},
    (uint16_t[]){1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 30, 36, 40, 45, 60, 72, 90, 120, 180, 360, 0},
    (uint16_t[]){2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 25, 30, 36, 40, 45, 50, 60, 72, 75, 90, 100, 120, 150, 180, 200, 225, 300, 360, 450, 600, 900, 1800, 0}
};

const int len = SIZEOF(harmonics);

regen_vars(play_arp,
    float phase, t;
    uint8_t harmonyIndex;
);
int play_arp(play_arp_state *self, uint8_t currentHarmony) {
    float dur = arpRhythm * secondsPerBeat;
    
    static int i = 0;
    // static uint8_t index = 0;
    // static int harmony = 0;
    static bool isChosen = false;
    regen_begin;
    self->t = 0;
    self->harmonyIndex = 0;

    for (;;) {

        // if (i > 75) {
        //     if (uniform(0, 1) < 0.5 && !isChosen) {
        //         currentHarmony[0] = uniform(0, len/2);
        //         isChosen = true;
        //         duty = 0.1;
        //         gate = 0.75;
        //     }
        // }

        for (; self->t < dur; self->t += dt) {
            if (harmonics[currentHarmony][self->harmonyIndex] == '\0') self->harmonyIndex = 0;
            float sig1 = pul(&self->phase, harmonics[currentHarmony][self->harmonyIndex] * FUND_FREQ, duty);

            // float sig2 = pul(&phases[1], harmonics[1][self->harmonyIndex] * FUND_FREQ, duty);
            // float sig3 = pul(&phases[2], harmonics[1][self->harmonyIndex] * FUND_FREQ, duty);
            // float orSig = (int)sig1 | (int)sig2 | (int)sig3;
            // yield(x * ad(t, dur/8, dur*7/8));
            reyield((int)(g(self->t, gate*dur) * sig1));
        }
        self->t -= dur;
        self->harmonyIndex++;
    }

    regen_end(0);

}

void set_harmony(int index, int which) {
    currentHarmonies[index] = which;
}

void set_harmonies(int max, int which) {
    for (int i = 0; i < max; i++) {
        currentHarmonies[i] = which;
    }
}

void set_all_harmonies(int which) {
    set_harmonies(9, which);
}

void set_half_harmonies(int which) {
    
    for (int i = 0; i < 9; i++) {
        if (i % 2 == 0)
            set_harmony(i, which);
    }
}

void set_percentage_harmonies(int which) {
    for (int i = 0; i < 9; i++) {
        float r = uniform(0, 1);
        bool isTrue = r < 0.5;
        if (isTrue) {
            set_harmony(i, which);
        }
    }
}

int changeHarmony() {
    static int i = 0;
    static float t = 0;
    static float dur = 1;
    gen_begin;
    for (; i < 60; i++) {
        for (; t < dur; t += dt) {
            switch(i) {
                case 0:
                    set_all_harmonies(0);
                    break;
                case 10:
                    set_all_harmonies(1);
                    break;
                case 21:
                    set_all_harmonies(0);
                    break;
                case 31:
                    // set_harmonies(4, 1);
                    set_percentage_harmonies(1);
                    // set_half_harmonies(1);
                    break;

            }
            yield(0);
        }
        t -= dur;
    }
    gen_end(0);
}

int changeGate() {
    static int i = 0;
    static float t = 0;
    static float dur = 1;
    gen_begin;
    for (; i < 60; i++) {
        for (; t < dur; t += dt) {
            if (i == 10) {
                gate = 0.5;
            } 
            else yield(0);
        }
        t -= dur;
    }
    gen_end(0);
}

void setup(unsigned int seed) {
    srand(seed);
}

static play_arp_state a={0}, b={0}, c={0}, d={0}, e={0}, f={0}, x={0}, y={0}, z={0};
float process() {

    // currentHarmony[0] = changeHarmony();

    // currentHarmony[0] = 1;
    // uint8_t aHarm = 0;
    // aHarm = changeHarmony();

    // changeGate();
    changeHarmony();

    return (
        play_arp(&a, currentHarmonies[0]) 
        | play_arp(&b, currentHarmonies[1])
        | play_arp(&c, currentHarmonies[2])
        | play_arp(&d, currentHarmonies[3])
        | play_arp(&e, currentHarmonies[4])
        | play_arp(&f, currentHarmonies[5])
        | play_arp(&x, currentHarmonies[6])
        | play_arp(&y, currentHarmonies[7])
        | play_arp(&z, currentHarmonies[8])
    );
}