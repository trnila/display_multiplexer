#pragma once

#define HALT() asm("bkpt #1"); for(;;);
#define ASSERT(cond) if(!cond) {HALT();}