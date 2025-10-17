#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct chunk {
    int start;
    int size;
    struct chunk* next;
} chunk;

chunk* head_chunk = NULL; // ���� ûũ (���⼭ �Ҵ��� �̷����)

chunk* create_my_chunk(int chunk_size) {
    chunk* head_chunk = malloc(sizeof(chunk));
    if (head_chunk == NULL) return NULL;

    head_chunk->start = 0;
    head_chunk->size = chunk_size;
    head_chunk->next = NULL;

    return head_chunk;
}

int myalloc(int request_size);
void myfree(int start_addr, int ret_size);
void print_chunks();

int main(void) {
    head_chunk = create_my_chunk(100);
    print_chunks();

    int a = myalloc(10);
    print_chunks();

    int b = myalloc(20);
    print_chunks();

    int c = myalloc(10);
    print_chunks();

    myfree(a, 10);
    print_chunks();

    a = myalloc(30);
    print_chunks();

    myfree(c, 10);
    print_chunks();

    c = myalloc(10);
    print_chunks();

    int d = myalloc(40);
    print_chunks();

    myfree(b, 20);
    print_chunks();

    int e = myalloc(20);
    print_chunks();

    myfree(e, 20);
    print_chunks();

    myfree(b, 20);
    myfree(a, 30);
    print_chunks();

    myfree(c, 10);
    print_chunks();

    return 0;
}

int myalloc(int request_size) {
    chunk* temp_chunk = head_chunk;
    chunk* prev_chunk = NULL;

    while (temp_chunk != NULL) { // first-fit: ����� ����ϰų� ���� ���� ã�� ��
        if (request_size > temp_chunk->size) { // ���� ������ ��� �ǳʶٱ�
            prev_chunk = temp_chunk;
            temp_chunk = temp_chunk->next;
            continue;
        }
        else { // ���� ���
            int alloc_addr = temp_chunk->start; // ��ȯ�� �ּ� ����

            temp_chunk->start += request_size;
            temp_chunk->size -= request_size;

            if (temp_chunk->size == 0) { // ������ ��� �Ҹ��� ���
                if (temp_chunk == head_chunk) {
                    head_chunk = temp_chunk->next;
                    free(temp_chunk);
                }
                else {
                    prev_chunk->next = temp_chunk->next;
                    free(temp_chunk);
                }
            }

            return alloc_addr;
        }
        prev_chunk = temp_chunk;
        temp_chunk = temp_chunk->next;
    }

    // while���� ������ �� ã�� ���
    printf("space not enough.\n");
    return -1;
}

void myfree(int start_addr, int ret_size) {
    chunk* temp_chunk = head_chunk;
    chunk* free_chunk = malloc(sizeof(chunk));
    chunk* prev_chunk = NULL; // �������鼭 ������ ûũ�� ���� �� ���

    if (free_chunk == NULL) return;

    free_chunk->start = start_addr;
    free_chunk->size = ret_size;
    
    //������ ���� ��� ûũ�� �ǳʶٴ� ����� �߰�. 
    prev_chunk = head_chunk;
    temp_chunk = temp_chunk->next;

    while (temp_chunk != NULL && temp_chunk->start < free_chunk->start) {
        prev_chunk = temp_chunk;
        temp_chunk = temp_chunk->next;
    } //free_chunk�� �� ��ġ�� ����. prev_chunk�� free_chunk �ڸ�

    if (temp_chunk == NULL) { //�׳� ���� ������ ��� ó��.
        prev_chunk->next = free_chunk;
        free_chunk->next = temp_chunk; //NULL
        return;
    }

    prev_chunk->next = free_chunk;
    free_chunk->next = temp_chunk;

    while (free_chunk!=NULL && prev_chunk->start + prev_chunk->size == free_chunk->start) {

        prev_chunk->size += free_chunk->size;
        prev_chunk->next = free_chunk->next;
        free(free_chunk);
        free_chunk = prev_chunk->next; //���� �ݺ��� ���� ����
    }
}

void print_chunks() {
    chunk* temp_chunk = head_chunk;
    int i = 0;

    while (temp_chunk != NULL) {
        printf("%d chunk: (start: %d, size: %d)\n", i++, temp_chunk->start, temp_chunk->size);
        temp_chunk = temp_chunk->next;
    }
    printf("\n");
}
