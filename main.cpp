#include <iostream>
#include "cpp-httplib/httplib.h"

void sd_card_write_data_with_len(char *full_path, const char *buf, size_t len, const char *mode, bool is_end);
int main() {
    httplib::Client cli("123.207.232.76", 81);

    std::string body;

    int total = 0;
    char *path="/home/vaca/https_file/gaga.rar";


    auto res = cli.Get("/chat3.rar",
                       [&](const char *data, size_t data_length) {
                           sd_card_write_data_with_len(path,data,data_length,"wb", false);
                           return true;
                       });
    sd_card_write_data_with_len(path,NULL,0,NULL, true);

}


#define BUFF_SIZE 4096
char write_buf[BUFF_SIZE];

void sd_card_write_data_with_len(char *full_path, const char *buf, size_t len, const char *mode, bool is_end) {
    static FILE *write_fp = NULL;
    static int write_index = 0;
    if (write_fp == NULL) {
        write_index = 0;
        write_fp = fopen(full_path, mode);
    }
    if (is_end) {
        if (write_index > 0) {
            fwrite(write_buf, 1, write_index, write_fp);
        }
        write_index = 0;
        fclose(write_fp);
        write_fp = NULL;
    } else {
        int buff_index=0;
        if(write_index==0){
            while (len >= (BUFF_SIZE+buff_index)) {
                fwrite(buf+buff_index, BUFF_SIZE, 1, write_fp);
                write_index=0;
                buff_index+=BUFF_SIZE;
            }
            if(buff_index!=len){
                write_index=len-buff_index;
                memcpy(write_buf,buf+buff_index,write_index);
            }
        }else{
            if(len+write_index<BUFF_SIZE){
                memcpy(write_buf+write_index,buf,len);
                write_index+=len;
            }else{
                buff_index=BUFF_SIZE-write_index;
                memcpy(write_buf+write_index,buf,buff_index);
                fwrite(write_buf,  1,BUFF_SIZE, write_fp);
                write_index=0;
                while (len >= (BUFF_SIZE+buff_index)) {
                    fwrite(buf+buff_index, BUFF_SIZE, 1, write_fp);
                    buff_index+=BUFF_SIZE;
                }
                if((buff_index)!=len){
                    write_index=len-buff_index;
                    memcpy(write_buf,buf+buff_index,write_index);
                }
            }
        }
    }
}