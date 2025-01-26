#include "ServiceTask.h"

class CloudServiceTask : public ServiceTask
{
    public:
        CloudServiceTask();
        bool init();
        void tick();
};

extern CloudServiceTask CloudService;