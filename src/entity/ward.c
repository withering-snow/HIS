#include <ward.h>

static long long _id_counter = 0;
static long long _id_counter2 = 0;

struct Ward_T {
    long long   id;
    Department  dept;
    long long   daily_cost;
    int         bed_count;
    int         empty_count;
    List_T      beds;
};

Ward_T Ward_load(
    long long id, Department dept, long long daily_cost)
{
    Ward_T w=safe_malloc(sizeof(struct Ward_T));
    w->id = id;
    w->dept=dept;
    w->daily_cost=daily_cost;
    w->bed_count=_id_counter;
    w->empty_count=_id_counter2;
    return w;
}

Ward_T Ward_new(
    Department dept, long long daily_cost,
    int start_bed_label, int bed_count)
{
    Ward_T w=safe_malloc(sizeof(struct Ward_T));
    Bed_T bed;
    w->dept=dept;
    w->daily_cost=daily_cost;
    w->bed_count=bed_count;
    bed.bed_label=start_bed_label;
    for(int i=0;i<bed_count;i++)
    {
        if(bed.status==1) w->empty_count++;
        else w->bed_count+=0;
    }
    return w;
}

void Ward_free(Ward_T* w)
{
    ASSERT(w !=NULL,"不合法");
    free(w);
    free(*w);
    *w=NULL;
}

long long   Ward_id(Ward_T w)
{
    ASSERT(w !=NULL,"不合法");
    return w->id;
}

Department  Ward_dept(Ward_T w)
{
    ASSERT(w !=NULL,"不合法");
    return w->dept;
}

long long   Ward_daily_cost(Ward_T w)
{
    ASSERT(w !=NULL,"不合法");
    return w->daily_cost;
}

int Ward_bed_count(Ward_T w)
{
    ASSERT(w !=NULL,"不合法");
    return w->bed_count;
}

int Ward_empty_count(Ward_T w)
{
    ASSERT(w !=NULL,"不合法");
    return w->empty_count;
}

Status Ward_update(Ward_T w, const Ward_Update_Pack* pack)
{
    ASSERT(w !=NULL,"不合法");
    w->dept=pack->dept;
    w->daily_cost=pack->daily_cost;
    return HIS_OK;
}

int Ward_cmp_id(const void* a, const void* b)
{
    Ward_T p=*(Ward_T *)a;
    Ward_T q=*(Ward_T *)b;
    return (p->id>q->id)-(p->id<q->id);
}

int Ward_cmp_dept(const void* a, const void* b)
{
    Ward_T p=*(Ward_T *)a;
    Ward_T q=*(Ward_T *)b;
    return (p->dept>q->dept)-(p->dept<q->dept);
}

int Ward_cmp_cost(const void* a, const void* b)
{
    Ward_T p=*(Ward_T *)a;
    Ward_T q=*(Ward_T *)b;
    return (p->daily_cost>q->daily_cost)-(p->daily_cost<q->daily_cost);
}

int Ward_cmp_empty(const void* a, const void* b)
{
    Ward_T p=*(Ward_T *)a;
    Ward_T q=*(Ward_T *)b;
    return (p->empty_count>q->empty_count)-(p->empty_count<q->empty_count);
}

Status Ward_bed_load(Ward_T w, int bed_label,
    long long pat_id, BedStatus status, long long start_ts)
{
    ASSERT(w !=NULL,"不合法");
    Bed_T bed;
    bed.bed_label=bed_label;
    bed.pat_id=pat_id;
    bed.status=status;
    bed.start_ts=start_ts;
    return HIS_OK;
}

Status Ward_occupy_bed(Ward_T w, int bed_label, long long pat_id)
{
    ASSERT(w !=NULL,"不合法");
    Bed_T bed;
    bed.bed_label=bed_label;
    bed.pat_id=pat_id;
    return HIS_OK;
}

Status Ward_free_bed(Ward_T w, int bed_label)
{
    ASSERT(w !=NULL,"不合法");
    Bed_T bed;
    bed.bed_label=bed_label;
    return HIS_OK;
}

Status Ward_release_bed(Ward_T w, long long pat_id)
{
    ASSERT(w !=NULL,"不合法");
    Bed_T bed;
    bed.pat_id=pat_id;
    return HIS_OK;
}

Bed_T* Ward_get_bed(Ward_T w, int bed_label)
{
    ASSERT(w !=NULL,"不合法");
    Bed_T* bed;
    bed->bed_label=bed_label;
    return bed;
}