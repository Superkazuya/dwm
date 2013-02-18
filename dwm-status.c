#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iwlib.h>
#include <mpd/client.h>
#include <alsa/asoundlib.h>
#include <X11/Xlib.h>

#define INTERVAL     1

#define MPD_ERROR   "\x03"
#define MPD_PLAYING "\x08"
#define NORMAL   "\x07"
#define MPD_STOP    "\x01"

#define MPD_PORT     6600
#define MPD_TIMEOUT  1000
#define TIME_FORMAT  "\x09%a \x07 %d/%m/%Y  \x02%H:\x01\x06%M"
#define WIFI	      "wlan0"
#define BATTERY	      "/sys/class/power_supply/BAT1/capacity"

void print_mpd_status(char * buffer)
{
  char mpd_info[100];
  struct mpd_connection *mpd_conn = mpd_connection_new("localhost", MPD_PORT, MPD_TIMEOUT);
  if(!mpd_conn)
  {
    strcat(buffer, MPD_ERROR "OOM");
    goto MPD_FREE;
  }
  if(mpd_connection_get_error(mpd_conn))
  {
    strcat(buffer, MPD_ERROR "ERR");
    goto MPD_FREE;
  }
  struct mpd_status *status = mpd_run_status(mpd_conn);
  if(status)
  {
    const enum mpd_state state = mpd_status_get_state(status);
    mpd_status_free(status);
    if(state == MPD_STATE_STOP)
    {
      strcat(buffer, MPD_STOP "救いは无い");
      goto MPD_FREE;
    }
    struct mpd_song *song = mpd_run_current_song(mpd_conn);
    if (song) 
    {
	const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	strcat(buffer, state == MPD_STATE_PLAY ? MPD_PLAYING : "\x02" );
	sprintf(mpd_info,	" %s", artist);
	strcat(buffer, mpd_info);

	const char *title  = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	strcat(buffer, "\x09\x01");
	//strcat(buffer, state == MPD_STATE_PLAY ? "\x01" : "\x01");
	sprintf(mpd_info,	" %s", title);
	strcat(buffer, mpd_info);
	mpd_song_free(song);
    }
  }
  else
  {
    strcat(buffer, MPD_ERROR"\x01 Fuck You ");
    goto MPD_FREE;
  }

MPD_FREE:
  mpd_connection_free(mpd_conn);
  return;
}

void print_battery(char * buffer, FILE* bat_file)
{
  char bat_char[8];
  int bat;
  if(!(bat_file = fopen(BATTERY,"r")))
    strcat(buffer, "fuck you");
  fscanf(bat_file, "%i\n", &bat);
  fclose(bat_file);
  sprintf(bat_char, "%d", bat);
  strcat(buffer, MPD_PLAYING "\x01");
  strcat(buffer, bat_char);
  strcat(buffer, "%");
}

void print_date(char * buffer)
{
  char buf[30];
  time_t rawtime = time(NULL);
  strftime(buf, sizeof(buf) - 1, TIME_FORMAT, localtime(&rawtime));

  strcat(buffer, buf);
  //printf(NORMAL "%s \n", buf);
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
  snd_mixer_open(&handle, 0);
  snd_mixer_attach(handle, "default");
  snd_mixer_selem_register(handle, NULL, NULL);
  snd_mixer_load(handle);
  snd_mixer_selem_id_t *vol_info; /* init channel with volume info */
  snd_mixer_selem_id_malloc(&vol_info);
  snd_mixer_selem_id_set_name(vol_info, "Master");
  snd_mixer_elem_t* pcm_mixer = snd_mixer_find_selem(handle, vol_info);
  snd_mixer_selem_get_playback_volume_range(pcm_mixer, &min, &max); /* get volume */
  snd_mixer_selem_get_playback_volume(pcm_mixer, SND_MIXER_SCHN_MONO, &vol);
  snd_mixer_selem_id_t *mute_info; /* init channel with mute info */
  snd_mixer_selem_id_malloc(&mute_info);
  snd_mixer_selem_id_set_name(mute_info, "Master");
  snd_mixer_elem_t* mas_mixer = snd_mixer_find_selem(handle, mute_info);
  snd_mixer_selem_get_playback_switch(mas_mixer, SND_MIXER_SCHN_MONO, &mute); /* get mute state */

  if(mute == 0)
    strcat(buffer, MPD_STOP);
  else
    strcat(buffer, NORMAL "\x01");
  realvol = (vol*100)/max;
  sprintf(vol_char, "%d", realvol);
  strcat(buffer, vol_char);
  strcat(buffer, "%");

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
  FILE *bat_file;
  Display *dpy;
  Window root;
  struct wireless_info *wl_info;
  wl_info = (struct wireless_info*) calloc(1, sizeof(struct wireless_info));

  if(!(dpy = XOpenDisplay(NULL)))
    exit(1);
  root = XRootWindow(dpy, DefaultScreen(dpy));

  while(1)
  {
    *buffer = '\0';
    print_mpd_status(buffer);
    print_vol(buffer);
    print_battery(buffer, bat_file);
    print_date(buffer);
    //printf(buffer);
    //fflush(stdout);
    XStoreName(dpy, root, buffer);
    XFlush(dpy);
    sleep(INTERVAL);
  }
  XCloseDisplay(dpy);
  exit(EXIT_SUCCESS);
}
