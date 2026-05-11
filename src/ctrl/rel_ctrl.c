#include <rel_ctrl.h>

static List_T Rel_doc_list = NULL;
static List_T Rel_ward_list = NULL;

static List_T Rel_appoint_queue = NULL;
static List_T Rel_waiting_queue = NULL;




void Rel_init(){
    Rel_doc_list  = List_new(sizeof(Rel_doc));
    Rel_ward_list = List_new(sizeof(Rel_ward));
    Rel_queue_update();
}

static void __queue_free(){
    if (Rel_appoint_queue != NULL) {
        Rel_queue* tmp = List_first(Rel_appoint_queue);
        while (tmp != NULL) {
            if (tmp->queue) {
                List_free(&(tmp->queue));
            }
            tmp = List_next(Rel_appoint_queue);
        }
        List_free(& Rel_appoint_queue);
    }

    if (Rel_waiting_queue != NULL){
        Rel_queue* tmp = List_first(Rel_waiting_queue);
        while (tmp != NULL){
            if (tmp->queue){
                List_free(&(tmp->queue));
            }
            tmp = List_next(Rel_waiting_queue);
        }
        List_free(& Rel_waiting_queue);
    }
}

void Rel_destroy(){
    List_free(& Rel_doc_list);
    List_free(& Rel_ward_list);
    __queue_free();
}

List_T Rel_doc_get(){
    return Rel_doc_list;
}

List_T Rel_ward_get(){
    return Rel_ward_list;
}


Status Rel_bind_doctor(long long patient_id, long long doctor_id){
    Rel_doc tmp = {patient_id, doctor_id};
    List_push_back(Rel_doc_list, &tmp);
    return HIS_OK;
}

Status Rel_unbind_doctor(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            List_remove(Rel_doc_list, tmp);
            return HIS_OK;
        }
        tmp = List_next(Rel_doc_list);
    }
    return HIS_ERR_NOT_FOUND;
}

long long Rel_get_doctor_by_patient(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return tmp->doc_id;
        }
        tmp = List_next(Rel_doc_list);
    }
    return HIS_ERR_NOT_FOUND;
}

List_T Rel_get_patients_by_doctor(long long doctor_id){
    List_T patients = List_new(sizeof(long long));
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->doc_id == doctor_id){
            List_push_back(patients, &(tmp->pat_id));
        }
        tmp = List_next(Rel_doc_list);
    }
    return patients;
}




Status Rel_bind_ward(long long patient_id, long long ward_id){
    Rel_ward tmp = {patient_id, ward_id};
    List_push_back(Rel_ward_list, &tmp);
    return HIS_OK;
}

Status Rel_unbind_ward(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            List_remove(Rel_ward_list, tmp);
            return HIS_OK;
        }
        tmp = List_next(Rel_ward_list);
    }
    return HIS_ERR_NOT_FOUND;
}

long long Rel_get_ward_by_patient(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return tmp->ward_id;
        }
        tmp = List_next(Rel_ward_list);
    }
    return HIS_ERR_NOT_FOUND;
}

List_T Rel_get_patients_by_ward(long long ward_id){
    List_T patients = List_new(sizeof(long long));
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->ward_id == ward_id){
            List_push_back(patients, &(tmp->pat_id));
        }
        tmp = List_next(Rel_ward_list);
    }
    return patients;
}




bool Rel_is_patient_admitted(long long patient_id){
    Rel_ward* tmp = List_first(Rel_ward_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return true;
        }
        tmp = List_next(Rel_ward_list);
    }
    return false;
}

bool Rel_has_doctor(long long patient_id){
    Rel_doc* tmp = List_first(Rel_doc_list);
    while(tmp != NULL){
        if(tmp->pat_id == patient_id){
            return true;
        }
        tmp = List_next(Rel_doc_list);
    }
    return false;
}




Status Rel_appoint_queue_push(long long doc_id, long long pat_id, int sequence_no, int time_frame){
    Rel_queue* tmp = List_first(Rel_appoint_queue);

    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node new_node = {
                pat_id, (time_frame * 10000) + sequence_no,
                sequence_no, time_frame}
            ;

            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                if(node->pat_id == pat_id){
                    return HIS_ERR_ALREADY_EXISTS;
                }
                if(node->priority_score >= new_node.priority_score){
                    break;
                }
                node = List_next(tmp->queue);
            }
            List_insert_before_cursor(tmp->queue, &new_node);

            return HIS_OK;
        }
        tmp = List_next(Rel_appoint_queue);
    }

    Rel_queue new_queue;
    new_queue.doc_id = doc_id;
    new_queue.queue = List_new(sizeof(Rel_queue_node));
    Rel_queue_node new_node = {
        pat_id, (time_frame * 10000) + sequence_no,
        sequence_no, time_frame}
    ;
    List_push_back(new_queue.queue, &new_node);
    List_push_back(Rel_appoint_queue, &new_queue);
    return HIS_OK;
}


Status Rel_waiting_queue_push(long long doc_id, long long pat_id, int sequence_no, int time_frame){
    Rel_queue* tmp = List_first(Rel_waiting_queue);

    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node new_node = {
                pat_id, (time_frame * 10000) + sequence_no,
                sequence_no, time_frame}
            ;

            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                if(node->pat_id == pat_id){
                    return HIS_ERR_ALREADY_EXISTS;
                }
                if(node->priority_score >= new_node.priority_score){
                    break;
                }
                node = List_next(tmp->queue);
            }
            List_insert_before_cursor(tmp->queue, &new_node);

            return HIS_OK;
        }
        tmp = List_next(Rel_waiting_queue);
    }

    Rel_queue new_queue;
    new_queue.doc_id = doc_id;
    new_queue.queue = List_new(sizeof(Rel_queue_node));
    Rel_queue_node new_node = {
        pat_id, (time_frame * 10000) + sequence_no,
        sequence_no, time_frame}
    ;
    List_push_back(new_queue.queue, &new_node);
    List_push_back(Rel_waiting_queue, &new_queue);
    return HIS_OK;
}




Status Rel_queue_remove(long long doc_id, long long pat_id){
    Rel_queue* tmp = List_first(Rel_appoint_queue);
    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                if(node->pat_id == pat_id){
                    List_remove(tmp->queue, node);
                    return HIS_OK;
                }
                node = List_next(tmp->queue);
            }
            break;
        }
        tmp = List_next(Rel_appoint_queue);
    }

    tmp = List_first(Rel_waiting_queue);
    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                if(node->pat_id == pat_id){
                    List_remove(tmp->queue, node);
                    return HIS_OK;
                }
                node = List_next(tmp->queue);
            }
            break;
        }
        tmp = List_next(Rel_waiting_queue);
    }
    return HIS_ERR_NOT_FOUND;
}




long long Rel_queue_call(long long doc_id){
    Rel_queue* tmp = List_first(Rel_waiting_queue);

    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node* node = List_first(tmp->queue);
            if(node != NULL){
                long long pat_id = node->pat_id;
                List_remove(tmp->queue, node);
                return pat_id;
            }
            break;
        }
        tmp = List_next(Rel_waiting_queue);
    }
    return INVALID_ID;
}


Status Rel_queue_check_in(long long doc_id, long long pat_id){
    Rel_queue* queue_ptr = List_first(Rel_appoint_queue);
    List_T queue = NULL;
    while(queue_ptr != NULL){
        if(queue_ptr->doc_id == doc_id){
            queue = queue_ptr->queue;
            break;
        }
        queue_ptr = List_next(Rel_appoint_queue);
    }
    if(queue == NULL){
        return HIS_ERR_NOT_FOUND;
    }

    Rel_queue_node* node_ptr = List_first(queue);
    Rel_queue_node tmp = {}; bool is_found = false;
    while(node_ptr != NULL){
        if(node_ptr->pat_id == pat_id){
            tmp = *node_ptr; is_found = true;
            List_remove(queue, node_ptr);
            break;
        }
        node_ptr = List_next(queue);
    }
    if(!is_found){
        return HIS_ERR_NOT_FOUND;
    }

    Rel_waiting_queue_push(doc_id, pat_id, tmp.sequence_no, tmp.time_frame);
    return HIS_OK;
}




List_T Rel_queue_get_appoint(long long doc_id){
    Rel_queue* tmp = List_first(Rel_appoint_queue);
    List_T list = List_new(sizeof(RelQueueDataPackage));

    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                RelQueueDataPackage pkg = {node->pat_id, node->sequence_no, node->time_frame};
                List_push_back(list, &pkg);
                node = List_next(tmp->queue);
            }
            break;
        }
        tmp = List_next(Rel_appoint_queue);
    }
    return list;
}


List_T Rel_queue_get_waiting(long long doc_id){
    Rel_queue* tmp = List_first(Rel_waiting_queue);
    List_T list = List_new(sizeof(RelQueueDataPackage));

    while(tmp != NULL){
        if(tmp->doc_id == doc_id){
            Rel_queue_node* node = List_first(tmp->queue);
            while(node != NULL){
                RelQueueDataPackage pkg = {node->pat_id, node->sequence_no, node->time_frame};
                List_push_back(list, &pkg);
                node = List_next(tmp->queue);
            }
            break;
        }
        tmp = List_next(Rel_waiting_queue);
    }
    return list;
}




void Rel_queue_update(){
    __queue_free();
    Rel_waiting_queue = List_new(sizeof(Rel_queue));
    Rel_appoint_queue = List_new(sizeof(Rel_queue));

    List_T records = Data_get_record();
    int today = Time_to_int_date(Time_now());
    void* find_ptr = List_first(records);
    while(find_ptr != NULL){
        Record_T record = *((Record_T*)find_ptr);

        if( Rec_is_invalid(record) == false &&
            Rec_type(record) == REC_REGISTRATION &&
            ((DataRegistration*)Rec_detail(record))->target_date == today)
        {
            DataRegistration* data = (DataRegistration*)Rec_detail(record);
            switch(data->status){
                case APPOINTMENT:
                    Rel_appoint_queue_push(data->doc_id, Rec_actor_id(record),
                        data->sequence_no, data->time_frame);
                    break;
                case WAITING:
                    Rel_waiting_queue_push(data->doc_id, Rec_actor_id(record),
                        data->sequence_no, data->time_frame);
                    break;
                default:
                    break;
            }
        }
        find_ptr = List_next(records);
    }
}
