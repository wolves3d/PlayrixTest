@ECHO OFF
FOR %%f IN (*.mp3) DO (
	ffmpeg.exe -i "%%f" -acodec libvorbis "%%f.ogg"
)
PAUSE