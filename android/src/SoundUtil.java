/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

package org.tophat;

import java.util.HashMap;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.content.Context;
import java.util.Queue;
import java.util.LinkedList;
import android.util.Log;

public class SoundUtil {
  private static HashMap<String, Integer> resources = new HashMap();

  static {
    resources.put("IDR_FAIL", R.raw.fail);
    resources.put("IDR_INSERT", R.raw.insert);
    resources.put("IDR_REMOVE", R.raw.remove);
    resources.put("IDR_WAV_BEEPBWEEP", R.raw.beep_bweep);
    resources.put("IDR_WAV_CLEAR", R.raw.beep_clear);
    resources.put("IDR_WAV_DRIP", R.raw.beep_drip);
  }
  private static MediaPlayer instance;

  static Boolean isPlaying = false;
  static Context _context;

  static Queue<Integer> queuedSounds = new LinkedList<Integer>();

  private static Boolean deQueueAndPlay() {
    synchronized (queuedSounds) {
      if (isPlaying) {
        return true;
      }

      Integer id = queuedSounds.poll();
      if (id == null) {
        return false;
      }

      isPlaying = true;
      isPlaying = playNow(_context, id);
    }
    return true;
  }

  /**
   * queues the sound and triggers play of the queue
   */
  public static boolean play(Context context, String name) {

    Integer id = resources.get(name);
    if (id == null) {
      Log.w(TAG, "SoundUtil::play error:  Resource not found: " + name);
      return false;
    }
    synchronized (queuedSounds) {
      queuedSounds.offer(id);
    }
    _context = context;

    return deQueueAndPlay();
  }

  private static boolean playNow(Context context, Integer id) {
    if (id == null) {
      Log.w(TAG, "SoundUtil::PlayNow error: id null");
      return false;
    }

    MediaPlayer mp = MediaPlayer.create(context, id);
    if (mp == null) {
      Log.w(TAG, "SoundUtil::PlayNow error.  Could not create Media Player. id: " + id);
      return false;
    }

    mp.setOnCompletionListener(new OnCompletionListener() {
	  @Override
	  public void onCompletion(MediaPlayer mp) {
		mp.release();
		mp = null;
		isPlaying = false;
        deQueueAndPlay();
      }
	});

    mp.start();
    return true;
  }
}
