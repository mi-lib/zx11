#define _POSIX_C_SOURCE 199309L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zx11/zxutil.h>
#include <zx11/zxinput.h>

typedef unsigned short LGCell;

#define LGCellState(c)          ( (c) & 0x4 )
#define LGCellNextState(c)      ( (c) & 0x8 )
#define LGCellStateOn(c)        ( (c) |= 0x4 )
#define LGCellNextStateOn(c)    ( (c) |= 0x8 )
#define LGCellStateOff(c)       ( (c) &= 0xb )
#define LGCellNextStateOff(c)   ( (c) &= 0x7 )
#define LGCellStateShift(c)     ( (c) >>= 1 )

typedef struct {
  unsigned rowsize, colsize;
  LGCell **cell;
  int settled;
} LGField;

int LGFieldAlloc(LGField *field, unsigned row, unsigned col)
{
  register int i;
  LGCell **cell;

  if( ( cell = (LGCell**)calloc( row+2, sizeof(LGCell*) ) ) == NULL )
    return 0;
  for( i=0; i<=row+1; i++ )
    if( ( *(cell+i) = (LGCell*)calloc( col+2, sizeof(LGCell) ) ) == NULL )
      return 0;
  field->rowsize = row;
  field->colsize = col;
  field->cell = cell;
  return 1;
}

void LGFieldFree(LGField *field)
{
  register int i;

  for( i=0; i<=field->rowsize+1; i++ )
    zFree( *(field->cell+i) );
  zFree( field->cell );
  field->rowsize = field->colsize = 0;
  field->cell = NULL;
}

void LGFieldGenRand(LGField *f, int rate)
{
  register int i, j;

  for( i=1; i<=f->rowsize; i++ )
    for( j=1; j<=f->colsize; j++ )
       if( ( rand() % 100 ) < rate ) LGCellStateOn( f->cell[i][j] );
}

void LGFieldUpdate(LGField *f)
{
  register int i, j, count;

  for( i=1; i<=f->rowsize; i++ )
    for( j=1; j<=f->colsize; j++ ){
      count  = LGCellState( f->cell[i-1][j-1] ) >> 2;
      count += LGCellState( f->cell[i-1][j  ] ) >> 2;
      count += LGCellState( f->cell[i-1][j+1] ) >> 2;
      count += LGCellState( f->cell[i  ][j-1] ) >> 2;
      count += LGCellState( f->cell[i  ][j+1] ) >> 2;
      count += LGCellState( f->cell[i+1][j-1] ) >> 2;
      count += LGCellState( f->cell[i+1][j  ] ) >> 2;
      count += LGCellState( f->cell[i+1][j+1] ) >> 2;
      if( LGCellState( f->cell[i][j] ) ){
        count >= 4 || count <= 1 ?
          LGCellNextStateOff( f->cell[i][j] ) :
          LGCellNextStateOn( f->cell[i][j] );
      } else{
        count == 3 ?
          LGCellNextStateOn( f->cell[i][j] ) :
          LGCellNextStateOff( f->cell[i][j] );
      }
    }
  for( f->settled=0, i=1; i<=f->rowsize; i++ )
    for( j=1; j<=f->colsize; j++ ){
      LGCellStateShift( f->cell[i][j] );
      if( ( LGCellState(f->cell[i][j]) >> 2 ^ f->cell[i][j] ) & 0x1 )
        f->settled++;
    }
}

#define CELL_W  8
#define CELL_H  8
#define CELL_R  3

ulong green;
ulong darkblue;

void LGFieldDraw(LGField *f, zxWindow *win)
{
  register int i, j;

  for( i=1; i<=f->rowsize; i++ )
    for( j=1; j<=f->colsize; j++ ){
      zxWindowSetColor( win, LGCellState( f->cell[i][j] ) ? green : darkblue );
      zxDrawFillRect( win,
        j*CELL_W+CELL_R-1, i*CELL_H+CELL_R-1, CELL_W-CELL_R, CELL_H-CELL_R );
    }
}

enum{
  LG_OPT_WIDTH=0,
  LG_OPT_HEIGHT,
  LG_OPT_RATE,
  LG_OPT_INTERVAL,
  LG_OPT_HELP,
};
zOption opt[] = {
  { "width", "width", "<width>", "set width of the field", (char *)"60", false },
  { "height", "height", "<height>", "set height of the field", (char *)"60", false },
  { "rate", "rate", "<percent>", "rate of initial population per field", (char *)"20", false },
  { "interval", "interval", "<delta time>", "interval time between generations", (char *)"50", false },
  { "h", "help", NULL, "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void LGUsage(char *arg)
{
  eprintf( "Usage: %s [options]\n", arg );
  zOptionHelp( opt );
  exit( 1 );
}

int main(int argc, char *argv[])
{
  zxWindow win;
  LGField field;
  int interval;
  struct timespec ts;
  bool quit_flag = false;

  if( argc < 1 ) LGUsage( argv[0] );
  zOptionRead( opt, argv, NULL );
  if( opt[LG_OPT_HELP].flag ) LGUsage( argv[0] );

  if( !LGFieldAlloc( &field, atoi( opt[LG_OPT_HEIGHT].arg ), atoi( opt[LG_OPT_WIDTH].arg ) ) )
    goto TERMINATE;
  srand( (int)time(NULL) );
  if( ( interval = atoi( opt[LG_OPT_INTERVAL].arg ) ) <= 0 ) interval = 50;
  ts.tv_sec = 0;
  ts.tv_nsec = interval * 1000 * 1000;

  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0,
    (field.colsize+2)*CELL_W, (field.rowsize+2)*CELL_H );
  zxWindowSetTitle( &win, "-- Life Game --" );
  zxWindowKeyEnable( &win );
  green = zxGetColor( &win, "green" );
  darkblue = zxGetColor( &win, "darkblue" );

  LGFieldGenRand( &field, atoi( opt[LG_OPT_RATE].arg ) );
  while( !quit_flag ){
    LGFieldUpdate( &field );
    if( field.settled > 0 ){
      LGFieldDraw( &field, &win );
      zxFlush();
    }
    if( zxDequeueEvent() == KeyPress ){
      switch( zxKeySymbol() ){
      case XK_q: case XK_Q: quit_flag = 1; break;
      case XK_i: case XK_I: LGFieldGenRand( &field, atoi( opt[LG_OPT_RATE].arg ) ); break;
      default: ;
      }
    }
    nanosleep( &ts, NULL );
  }
 TERMINATE:
  LGFieldFree( &field );
  zxExit();
  return 1;
}
