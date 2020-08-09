from lxml import html
import spotipy.util as util
import spotipy
import requests
import time


def SetColor(RedValue, GreenValue, BlueValue):
    try:
        StartTime = time.time()

        Page = "http://89.136.241.70:243/?r" + str(RedValue) + "g" + str(GreenValue) + "b" + str(BlueValue) + "&"
        html.fromstring(requests.get(Page).content)
        print("Set Color in", round(time.time() - StartTime, 3), "Seconds > R:", RedValue, "G:", GreenValue, "B:", BlueValue)
    except:
        print("Error in sending request")


def SetAnimation(DelayOne, DelayTwo):
    try:
        StartTime = time.time()
        Page = "http://89.136.241.70:243/?r256g" + str(DelayOne) + "b" + str(DelayTwo) + "&"
        html.fromstring(requests.get(Page).content)
        print("Set Animation in", round(time.time() - StartTime, 3), "Seconds > Delay One:", DelayOne, "Delay Two", DelayTwo)
    except:
        print("Error in sending request")


def SpotifyAuth():
    Scope = "user-read-currently-playing"
    Username = "Dexter"
    ClientID = 'ff0e8504320649d9a1fe1e6b0b422ae4'
    ClientSecret = '537b2f9dc29348b680cc3bdda64bab0d'
    RedirectURL = 'https://dexter0-0.github.io/'
    return spotipy.Spotify(auth=util.prompt_for_user_token(Username, Scope, ClientID, ClientSecret, RedirectURL))


def PrintSongInfo(CurrentSongInfo, CurrentSongState, BPM):
    CurrentSongName = CurrentSongInfo["item"]["name"]
    CurrentSongArtists = CurrentSongInfo["item"]["artists"]

    if len(CurrentSongArtists) > 1:
        CurrentSongArtist = ""
        for i in range(len(CurrentSongArtists)):
            CurrentSongArtist = CurrentSongArtist + CurrentSongArtists[i]["name"] + " , "
    else:
        CurrentSongArtist = CurrentSongArtists[0]["name"]

    print("")
    print("                             | SONG UPDATE |")
    print("")
    print("You are listening to >", CurrentSongName, "by:", CurrentSongArtist, "with BPM:", BPM, "Current state:", CurrentSongState)


if __name__ == '__main__':
    Spotify = SpotifyAuth()
    LastTime = time.time()
    SongStartTime = time.time()
    CurrentSongID = "null"
    BPM = 0
    SongUpdateCounter = 0
    StoppedSong = False
    WhiteColorSet = False

    while True:
        if time.time() - LastTime >= 2:
            LastTime = time.time()

            print("")
            print("Getting song update (", SongUpdateCounter, ") at :", round(LastTime - SongStartTime, 2),
                  "Seconds since song start")

            try:
                LastChekedSong = CurrentSongID
                CurrentSongInfo = Spotify.current_user_playing_track()

                CurrentSongID = CurrentSongInfo["item"]["id"]
                CurrentSongState = CurrentSongInfo["is_playing"]

                print("Got song update (", SongUpdateCounter, ") after:", round(time.time() - LastTime, 2), "Seconds")

                if CurrentSongState is False and StoppedSong is False:
                    if WhiteColorSet is False:
                        SetColor(255, 255, 255)
                        WhiteColorSet = True

                    PrintSongInfo(CurrentSongInfo, CurrentSongState, BPM)
                    StoppedSong = True

                if CurrentSongState is True and StoppedSong is True:
                    SetAnimation(0, 60000 / BPM)
                    WhiteColorSet = False
                    PrintSongInfo(CurrentSongInfo, CurrentSongState, BPM)
                    StoppedSong = False

                if LastChekedSong != CurrentSongID:
                    SongStartTime = time.time()
                    SongUpdateCounter = 0
                    AudioFeatures = Spotify.audio_features(CurrentSongID)[0]
                    AudioAnalysis = Spotify.audio_analysis(CurrentSongID)
                    BPM = AudioFeatures["tempo"]

                    PrintSongInfo(CurrentSongInfo, CurrentSongState, BPM)
                    # PrintSongSections(AudioAnalysis)

                    SetAnimation(0, 60000 / BPM)
                    WhiteColorSet = False

                    print("Animation started with delay:", CurrentSongInfo["progress_ms"] / 1000, "Seconds since song started")
                    print("")
            except spotipy.client.SpotifyException:
                if WhiteColorSet is False:
                    SetColor(255, 255, 255)
                    WhiteColorSet = True
                print("Getting new token...")
                Spotify = SpotifyAuth()
                CurrentSongID = "null"
            except:
                print("Error in getting a song update / getting song info")
                if WhiteColorSet is False:
                    SetColor(255, 255, 255)
                    WhiteColorSet = True

            SongUpdateCounter = SongUpdateCounter + 1
        time.sleep(0.1)
