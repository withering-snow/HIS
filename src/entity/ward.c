#include <ward.h>




static long long _id_counter = 0;




struct Ward_T {
    // 8 bytes
    long long   id;
    long long   daily_cost;
    List_T      beds;

    // 4 bytes
    Department  dept;
    int         bed_count;
    int         empty_count;

    // char*
    char        ward_name[32];
};




Ward_T Ward_load(long long id, Department dept, long long daily_cost, const char* ward_name){
    LOAD_ID(id);
    Ward_T w = (Ward_T)safe_malloc(sizeof(struct Ward_T));
    w->id = id;
    w->dept = dept;
    w->daily_cost = daily_cost;
    w->bed_count = 0;
    w->empty_count = 0;
    w->beds = List_new(sizeof(Bed_T));
    strncpy(w->ward_name, ward_name, sizeof(w->ward_name));
    return w;
}

Ward_T Ward_new(Department dept, long long daily_cost, int start_bed_label, int bed_count, const char* ward_name){
    long long id = NEW_ID();
    Ward_T w = (Ward_T)safe_malloc(sizeof(struct Ward_T));
    w->id = id;
    w->dept = dept;
    w->daily_cost = daily_cost;
    w->bed_count = bed_count;
    w->empty_count = bed_count;
    strncpy(w->ward_name, ward_name, sizeof(w->ward_name));

    List_T beds = List_new(sizeof(Bed_T));
    for(int i = 0; i < bed_count; i++){
        Bed_T tmp;
        tmp.bed_label = start_bed_label + i;
        tmp.pat_id = INVALID_ID;
        tmp.status = BED_EMPTY;
        tmp.start_ts = INVALID_TIME;
        List_push_back(beds, &tmp);
    }
    w->beds = beds;

    return w;
}

void Ward_free(Ward_T*w){
    ASSERT(w != NULL && *w != NULL, "空指针释放");
    List_free(&((*w)->beds));
    free(*w);
    *w = NULL;
}




long long Ward_id(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->id;
}

Department Ward_dept(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->dept;
}

long long Ward_daily_cost(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->daily_cost;
}

int Ward_bed_count(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->bed_count;
}

int Ward_empty_count(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->empty_count;
}

List_T Ward_beds(Ward_T w){
    ASSERT(w!=NULL, "空指针访问");
    return w->beds;
}

const char * Ward_name(Ward_T w){
    return w->ward_name;
}




Status Ward_update(Ward_T w, const Ward_Update_Pack *pack){
    ASSERT(w!=NULL && pack != NULL, "空指针访问");
    w->dept = pack->dept;
    w->daily_cost = pack->daily_cost;
    strncpy(w->ward_name, pack->ward_name, sizeof(w->ward_name));
    return HIS_OK;
}




int Ward_cmp_id(const void *a, const void *b) {
    Ward_T p = *(Ward_T *)a;
    Ward_T q = *(Ward_T *)b;
    return (p->id > q->id) - (p->id < q->id);
}

int Ward_cmp_dept(const void *a, const void *b) {
    Ward_T p = *(Ward_T *)a;
    Ward_T q = *(Ward_T *)b;
    return (p->dept > q->dept) - (p->dept < q->dept);
}

int Ward_cmp_cost(const void *a, const void *b) {
    Ward_T p = *(Ward_T *)a;
    Ward_T q = *(Ward_T *)b;
    return (p->daily_cost > q->daily_cost) - (p->daily_cost < q->daily_cost);
}

int Ward_cmp_empty(const void *a, const void *b) {
    Ward_T p = *(Ward_T *)a;
    Ward_T q = *(Ward_T *)b;
    return (p->empty_count > q->empty_count) - (p->empty_count < q->empty_count);
}

int Ward_cmp_name(const void *a, const void *b){
    Ward_T p = *(Ward_T *)a;
    Ward_T q = *(Ward_T *)b;
    return strncmp(p->ward_name, q->ward_name, sizeof(p->ward_name));
}


int Ward_cmp_fuzzy(const void *a, const void *b){
    Ward_T p = *(Ward_T *)a;
    if(strstr(p->ward_name, (const char*)b) != NULL){
        return HIS_OK;
    }
    return HIS_ERR_NOT_FOUND;
}




Status Ward_bed_load(Ward_T w, int bed_label, long long pat_id, BedStatus status, long long start_ts){
    ASSERT(w!=NULL, "空指针访问");
    Bed_T tmp;
    tmp.bed_label = bed_label;
    tmp.pat_id = pat_id;
    tmp.status = status;
    tmp.start_ts = start_ts;

    w->bed_count++;
    if(status == BED_EMPTY){
        w->empty_count++;
    }

    List_push_back(w->beds, &tmp);
    return HIS_OK;
}




Status Ward_occupy_bed(Ward_T w, int bed_label, long long pat_id){
    ASSERT(w!=NULL, "空指针访问");

    Bed_T *tmp = (Bed_T *)List_first(w->beds);
    while(tmp != NULL){
        if(tmp->bed_label == bed_label){

            if(tmp->status != BED_EMPTY){
                return HIS_ERR_BED_OCCUPIED;
            }

            tmp->pat_id = pat_id;
            tmp->status = BED_OCCUPIED;
            tmp->start_ts = Time_now();
            w->empty_count--;
            return HIS_OK;
        }
        tmp = (Bed_T *)List_next(w->beds);
    }
    return HIS_ERR_NOT_FOUND;
}

Status Ward_vacate_by_bed(Ward_T w, int bed_label){
    ASSERT(w!=NULL, "空指针访问");

    Bed_T *tmp = (Bed_T *)List_first(w->beds);
    while(tmp != NULL){
        if(tmp->bed_label == bed_label){
            if (tmp->status != BED_EMPTY){
                w->empty_count++;
            }
            tmp->pat_id = INVALID_ID;
            tmp->status = BED_EMPTY;
            tmp->start_ts = INVALID_TIME;
            return HIS_OK;
        }
        tmp = (Bed_T *)List_next(w->beds);
    }
    return HIS_ERR_NOT_FOUND;
}

Status Ward_vacate_by_patient(Ward_T w, long long pat_id){
    ASSERT(w!=NULL, "空指针访问");

    Bed_T *tmp = (Bed_T *)List_first(w->beds);
    while(tmp != NULL){
        if(tmp->pat_id == pat_id){
            if (tmp->status != BED_EMPTY){
                w->empty_count++;
            }
            tmp->pat_id = INVALID_ID;
            tmp->status = BED_EMPTY;
            tmp->start_ts = INVALID_TIME;
            return HIS_OK;
        }
        tmp = (Bed_T *)List_next(w->beds);
    }
    return HIS_ERR_NOT_FOUND;
}

Bed_T * Ward_get_bed(Ward_T w, int bed_label){
    ASSERT(w!=NULL, "空指针访问");

    Bed_T *tmp = (Bed_T *)List_first(w->beds);
    while(tmp != NULL){
        if(tmp->bed_label == bed_label){
            return tmp;
        }
        tmp = (Bed_T *)List_next(w->beds);
    }
    return NULL;
}

Bed_T * Ward_get_bed_by_patient(Ward_T w, long long pat_id){
    ASSERT(w!=NULL, "空指针访问");

    Bed_T *tmp = (Bed_T *)List_first(w->beds);
    while(tmp != NULL){
        if(tmp->pat_id == pat_id){
            return tmp;
        }
        tmp = (Bed_T *)List_next(w->beds);
    }
    return NULL;
}
