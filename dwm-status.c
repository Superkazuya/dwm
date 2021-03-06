#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iwlib.h>
#include <mpd/client.h>
#include <alsa/asoundlib.h>
#include <X11/Xlib.h>

#define INTERVAL     1

//#define MPD_PORT     6600
//#define MPD_TIMEOUT  1000
//#define TIME_FORMAT  "\x09%a \x07 %d/%m/%Y  \x02%H:\x01\x06%M"
#define TIME_FORMAT  "\x0f%a %d/%m/%Y\x10%H:%M"
//#define WIFI	      "wlsp02"
#define BATTERY	      "/sys/class/power_supply/BAT1/capacity"

void print_battery(char * buffer)
{
  FILE *bat_file;
  char bat_char[8];
  int bat;
  assert((bat_file = fopen(BATTERY,"r")) != 0);
  fscanf(bat_file, "%d\n", &bat);
  fclose(bat_file);
  sprintf(bat_char, "\u2B83 %d%%", bat);
  strcat(buffer, bat_char);
}

void print_date(char * buffer)
{
  char buf[64];
  time_t rawtime = time(NULL);
  strftime(buf, sizeof(buf) - 1, TIME_FORMAT, localtime(&rawtime));
  strcat(buffer, buf);
}

void print_vol(char * buffer)
{
  int mute = 0;
  int realvol = 0;
  long min = 0;
  long max = 0;
  long vol = 0;
  char vol_char[8];
  snd_mixer_t *handle; /* init alsa */
  snd_mixer_selem_id_t *mute_info; /* init channel with mute info */
  snd_mixer_elem_t* mas_mixer; 

  snd_mixer_open(&handle, 0);
  snd_mixer_attach(handle, "default");
  snd_mixer_selem_register(handle, NULL, NULL);
  snd_mixer_load(handle);
  snd_mixer_selem_id_t *vol_info; /* init channel with volume info */
  snd_mixer_selem_id_malloc(&vol_info);
  snd_mixer_selem_id_set_name(vol_info, "Master");
  snd_mixer_elem_t* pcm_mixer = snd_mixer_find_selem(handle, vol_info);
  if(pcm_mixer == NULL)
    goto ERROR;
  snd_mixer_selem_get_playback_volume_range(pcm_mixer, &min, &max); /* get volume */
  snd_mixer_selem_get_playback_volume(pcm_mixer, SND_MIXER_SCHN_MONO, &vol);
  snd_mixer_selem_id_malloc(&mute_info);
  snd_mixer_selem_id_set_name(mute_info, "Master");
  mas_mixer = snd_mixer_find_selem(handle, mute_info);
  snd_mixer_selem_get_playback_switch(mas_mixer, SND_MIXER_SCHN_MONO, &mute); /* get mute state */

  if(mute != 0)
    strcat(buffer, "\u2b83 ");
  realvol = (vol*100)/max;
  sprintf(vol_char, "%d%% ", realvol);
  strcat(buffer, vol_char);

ERROR:
  if(vol_info)
    snd_mixer_selem_id_free(vol_info);
  if (mute_info)
    snd_mixer_selem_id_free(mute_info);
  if (handle)
    snd_mixer_close(handle);
}


int main()
{
  char buffer[256];
  Display *dpy;
  Window root;
  /*
  if(fork())
    exit(EXIT_SUCCESS);
  struct wireless_info *wl_info;
  wl_info = (struct wireless_info*) calloc(1, sizeof(struct wireless_info));
  */

  if(!(dpy = XOpenDisplay(NULL)))
    exit(1);
  root = XRootWindow(dpy, DefaultScreen(dpy));

  while(1)
  {
    *buffer = '\0';
    //Init 
    print_vol(buffer);
    print_battery(buffer);
    print_date(buffer);
    fflush(stdout);
    XStoreName(dpy, root, buffer);
    XFlush(dpy);
    sleep(INTERVAL);
  }
  XCloseDisplay(dpy);
  exit(EXIT_SUCCESS);
}
