#pragma once

#define HALT() asm("bkpt #1"); for(;;);