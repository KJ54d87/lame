#include "lame.h"
#include "lame.c"

int main (){
FILE *pcm = fopen("BadApple.mp3", "rb");
FILE *mp3 = fopen("out.mp3", "wb");
size_t nread;
int ret, nwrite;

// 1. Get lame version (OPTIONAL)
printf("Using LAME v%s\n", get_lame_version());

const int PCM_SIZE = 8192;
const int MP3_SIZE = 8192;

short pcm_buffer[PCM_SIZE * 2];
unsigned char mp3_buffer[MP3_SIZE];

// 2. Initializing
lame_t lame = lame_init();

// 3. Do some settings (OPTIONAL)
// lame_set_in_samplerate(lame, 48000);
lame_set_VBR(lame, vbr_default);
// lame_set_VBR_quality(lame, 2);

// 4. Initialize parameters
ret = lame_init_params(lame);
if (ret < 0) {
    printf("Error occurred during parameters initializing. Code = %d\n",
            ret);
    return 1;
}

do {
    // Read PCM_SIZE of array
    nread = fread(pcm_buffer, 2 * sizeof(short), PCM_SIZE, pcm);
    if (nread != 0) {
        // 5. Encode
        int nsamples = nread;
        short buffer_l[nsamples];
        short buffer_r[nsamples];

        printf("nread = %d\n", nread);
        printf("pcm_buffer.length = %d\n", sizeof(pcm_buffer)/sizeof(short));

        int j = 0;
        int i = 0;
        for (i = 0; i < nsamples; i++) {
            buffer_l[i] = pcm_buffer[j++];
            buffer_r[i] = pcm_buffer[j++];

        }

        nwrite = lame_encode_buffer(lame, buffer_l, buffer_r, nread,
                mp3_buffer, MP3_SIZE);
    } else {
        // 6. Flush and give some final frames
        nwrite = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
    }

    if (nwrite < 0) {
        printf("Error occurred during encoding. Code = %d\n", nwrite);
        return 1;
    }

    fwrite(mp3_buffer, nwrite, 1, mp3);
} while (nread != 0);

// 7. Write INFO tag (OPTIONAL)
// lame_mp3_tags_fid(lame, mp3);

// 8. Free internal data structures
lame_close(lame);

fclose(mp3);
fclose(pcm);
return 0;
}