## About

Skating spin spinner is a simple project that attempts to spin random figure skating spins that will follow ISU (international skating union) rules. It is intended to layout the required logic that then can be used in graphical projects. See [2025-26 Levels of Difficulty Singles.pdf](2025-26 Levels of Difficulty Singles.pdf) for rule details.

## Setup

To use the spin spinner cli, download from [releases](https://github.com/jsfischer343/skating-spin-spinner-cli/releases) or compile from source. Then either run the binary directly or add it to your PATH.

## Terms

Direction: Refers to the rotational direction of the spin, either clockwise or counterclockwise.
Footness: Refers to foot that is spun on, either dominant (counterclockwise on right foot, clockwise on left foot) or non-dominant (clockwise on right foot, counterclockwise on left foot).
Position: Camel, sit, upright, or layback that act as a base components of spins. There is also a "non-basic" or intermediate position that can appear in combo spins.
Segment: A series of positions all on the same foot. Change foot spins will have two segments, all other spins will only have one.
(Difficult) Variation: A change of core or body position inside of a spin position that changes the quality of the spin in a significant way.
Feature: A special modification to the nature of a spin that makes it more difficult. In some contexts a difficult variation is also considered a feature.

## Format

Level x: (direction)\[ footness position variation & variation (features) + ...\] + (direction)\[ footness position variation & variation (features) + ...\] with difficult entry/exit

---

Examples:
`
Level 3: (ccw)[ back layback side(8revs) ] with difficult exit
Level 3: (ccw)[ back camel (8revs, coe) ] + (ccw)[ forward camel front & side ]
Level 2: flying (ccw)[ forward camel  + sit  + upright  ] --jump-- (ccw)[ back sit  + camel  + upright  ]
Level 1: (ccw)[ forward sit  ] + (ccw)[ back sit front ]
`

## Credits

Argparser: [argparser.hh](src/argparser.hh) from https://github.com/fmenozzi/argparser

## AI Disclaimer

All original code and content in this repository is written line by line using available documentation or leveraging existing libraries. No use of AI or "agentic" coding tools was used to write the original code in this repository.
