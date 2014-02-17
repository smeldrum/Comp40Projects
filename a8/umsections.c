#include "umsections.h"
void free_sections(const void* key, void **value, void * cl);
typedef struct Umsections_T {
    Table_T sections;
    Seq_T current_sec;
    int (*error)(void *errstate, const char *message);
    void* errstate;
}*Umsections_T;


Umsections_T Umsections_new(const char *section, 
            int (*error)(void *errstate, const char *message),
            void *errstate){
   Umsections_T asm=malloc(sizeof(struct Umsections_T));
   asm->sections=Table_new(1000, NULL, NULL);
   current_sec=Seq_new(100);
   Table_put(asm->sections,Atom_new(section, strlen(section)),current_sec);
   asm->error=error;
   asm->errstate=errstate;
   return asm;
}
void Umsections_free(Umsections_T *asmp){
    Umsections_T temp=*asmp;
    Table_T table=temp->sections;
    Table_map(temp, free_sections, NULL);
    Table_free(&table);
}

void free_sections(const void* key, void **value, void * cl){
    Seq_T * sequence=(Seq_T*)value;
    Seq_free(sequence);
    (void)key;
    (void)cl;
}
