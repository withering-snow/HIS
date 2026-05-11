#include <medicine.h>
static long long _id_counter=1;
static long long _next_bat_id = 1;

#define BAT_ID_NEW()     ((_next_bat_id < LLONG_MAX)? (_next_bat_id++): INVALID_ID)

// 加载批次：设置起始 ID (对应 ID_LOAD)
#define BAT_ID_LOAD(id) \
do{ \
if(id > _next_bat_id) _next_bat_id = id; \
}while(0)

struct Medicine_T {
    // 8 bytes
    long long   id;
    long long   cur_price;
    List_T      batches;

    // 4 bytes
    int         total_remain;

    // char*
    char        name[32];
};

Medicine_T Medicine_load(
    long long id, long long cur_price,
    int total_remain, const char* name)
{
    Medicine_T m=safe_malloc(sizeof(Medicine_T));
    LOAD_ID(id);
    m->id=id;
    m->cur_price=cur_price;
    m->total_remain=total_remain;
    strncpy(m->name,name,32);
    m->batches = List_new(sizeof(MedicineBatch));
    return m;
}

Medicine_T Medicine_new(
    long long cur_price,
    const char* name)
{
    Medicine_T m=safe_malloc(sizeof(Medicine_T));
    ASSERT(m !=NULL,"不合法");
    m->id=NEW_ID();
    m->cur_price=cur_price;
    strncpy(m->name,name,32);
    m->batches = List_new(sizeof(MedicineBatch));
    return m;
}


void Medicine_free(Medicine_T *m)
{
    ASSERT(m !=NULL,"不合法");
    free(*m);
    *m=NULL;
}


long long Medicine_id(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    return m->id;
}


long long Medicine_cur_price(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    return m->cur_price;
}


int Medicine_total_remain(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    return m->total_remain;
}


const char* Medicine_name(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    return m->name;
}


List_T Medicine_batches(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    return m->batches;
}

Status Medicine_set_price(Medicine_T m, int new_price)
{
    ASSERT(m !=NULL,"不合法");
    m->cur_price=new_price;
    return HIS_OK;
}

static void refresh_total_remain(Medicine_T m) {
    ASSERT(m != NULL,"不合法");
    int total = 0;
    MedicineBatch *bat = List_first(m->batches);
    while (bat != NULL) {
        if (bat->status == AVAILABLE) {
            total += bat->remain;
        }
        bat = List_next(m->batches);
    }
    m->total_remain = total;
}

Status Medicine_update_remain(Medicine_T m)
{
    ASSERT(m !=NULL,"不合法");
    refresh_total_remain(m);
    return HIS_OK;
}


Status Medicine_set_name(Medicine_T m, const char* new_name)
{
    ASSERT(m !=NULL,"不合法");
    strncpy(m->name,new_name,32);
    return HIS_OK;
}


int Medicine_cmp_id(const void* a, const void* b)
{
    Medicine_T p=*(Medicine_T *)a;
    Medicine_T q=*(Medicine_T *)b;
    return (p->id>q->id)-(p->id<q->id);
}


int Medicine_cmp_remain(const void* a, const void* b)
{
    Medicine_T p=*(Medicine_T *)a;
    Medicine_T q=*(Medicine_T *)b;
    return (p->total_remain>q->total_remain)-(p->total_remain<q->total_remain);
}


int Medicine_cmp_fuzzy(const void* a, const void* b)
{
    Medicine_T p=*(Medicine_T *)a;
    if (strstr(p->name,b)!=NULL)
        return 0;
    else  return HIS_ERR_NOT_FOUND;
}


Status Medicine_batch_load(
    Medicine_T m,
    long long id, long long buy_price, long long expire_ts,
    int remain, BatchStatus status, const char* batch_no)
{
    ASSERT(m !=NULL,"不合法");
    MedicineBatch bat;
    bat.id=id;
    bat.buy_price=buy_price;
    bat.expire_ts=expire_ts;
    bat.remain=remain;
    bat.status=status;
    strncpy(bat.batch_no, batch_no, 31);
    bat.batch_no[31] = '\0';
    List_push_back(m->batches,&bat);
    return HIS_OK;
}

Status Medicine_batch_add(
    Medicine_T m,
    long long buy_price, long long expire_ts,
    int remain, const char* batch_no)
{
    ASSERT(m != NULL, "不合法");
    ASSERT(remain > 0, "批次库存必须大于0");

    MedicineBatch bat;
    bat.id = BAT_ID_NEW();
    bat.buy_price = buy_price;
    bat.expire_ts = expire_ts;
    bat.remain    = remain;
    bat.status    = AVAILABLE;
    strncpy(bat.batch_no, batch_no, 31);
    bat.batch_no[31] = '\0';
    List_push_back(m->batches, &bat);
    Medicine_update_remain(m);
    return HIS_OK;
}

Status Medicine_deduct(Medicine_T m, int amount)
{
    ASSERT(m != NULL, "不合法");
    ASSERT(amount > 0, "扣减数量必须大于0");
    int total = Medicine_total_remain(m);
    if (total < amount) {
        return HIS_ERR_OUT_OF_STOCK;
    }
    List_sort(m->batches, Medicine_batch_cmp_expire);
    int need = amount;
    MedicineBatch* bat = List_first(m->batches);
    while (bat != NULL && need > 0) {
        if (bat->remain > 0) {
            int deduct = (bat->remain < need) ? bat->remain : need;
            bat->remain -= deduct;
            need -= deduct;
            if (bat->remain <= 0) {
                bat->status = EXHAUSTED;
            }
        }
        bat = List_next(m->batches);
    }

    Medicine_update_remain(m);
    return HIS_OK;
}


Status Medicine_discard_batch(Medicine_T m, long long batch_id)
{
    ASSERT(m != NULL, "不合法");
    ASSERT(batch_id > 0, "批次ID不合法");
    MedicineBatch search;
    search.id = batch_id;
    MedicineBatch* bat = (MedicineBatch*)List_find(
        m->batches,&search,Medicine_batch_cmp_id);
    if (bat == NULL) {
        return HIS_ERR_NOT_FOUND;
    }
    if (bat->status == DISCARD) {
        return HIS_OK;
    }
    bat->status = DISCARD;
    bat->remain = 0;
    Medicine_update_remain(m);
    return HIS_OK;
}


int Medicine_batch_cmp_expire(const void* a, const void* b)
{
    const MedicineBatch *p=(const MedicineBatch *)a;
    const MedicineBatch *q=(const MedicineBatch *)b;
    return (p->expire_ts>q->expire_ts)-(p->expire_ts<q->expire_ts);
}


int Medicine_batch_cmp_id(const void* a, const void* b)
{
    const MedicineBatch *p=(const MedicineBatch *)a;
    const MedicineBatch *q=(const MedicineBatch *)b;
    return (p->id>q->id)-(p->id<q->id);
}


int Medicine_batch_cmp_remain(const void* a, const void* b)
{
    const MedicineBatch *p=(const MedicineBatch *)a;
    const MedicineBatch *q=(const MedicineBatch *)b;
    return (p->remain>q->remain)-(p->remain<q->remain);
}