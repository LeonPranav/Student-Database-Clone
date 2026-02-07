#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>

class courses{
    public:
        std::string cid;                                                                           //course id
        std::string cname;                                                                        //name of course 
        int credits,capacity;
        int wcount;                                                                              //wcount used to count no. of ppl in waitlist
        std::vector <std::string> prereq;                                                        //used to store prerequisites
        std::vector <std::string> students;                                                       //stores enrolled students
        std::vector <std::string> waitlist;
        char slot;
};

class stud{
    public:
        std::string sid;                                //student id
        std::string sname;                                   //student name
        int year;                                           //year of studying in college
        std::vector <std::string> complete;                 //stores completed courses
        std::vector <std::string> enrolled;                 //stores enrolled courses
};

void enroll(courses* c[],stud* s[],int ccount,int scount,std::string cid,std::string sid){
    //std::cout<<"Entered enroll function"<<std::endl;
    int t1,low1=0,high1=ccount-1;                   //t1 stores index of cid in courses pointer array
    while(low1<=high1){                             //i could have made a function for binary search but by the time i realized i almost didnt need it anymore so...
        t1=(low1+high1)/2;
        if(c[t1]->cid>cid)
        high1=t1-1;
        else if(c[t1]->cid<cid)
        low1=t1+1;
        else
        break;
    }
    if(low1==ccount || high1==-1){
        //std::cout<<1<<std::endl;
        return;
    }
    if((c[t1])->capacity==0){                                //adding student to waitlist if capacity is over
        (c[t1]->waitlist).push_back(sid);
        (c[t1]->wcount)++;
        return;
    }
    //std::cout<<"Checkpoint 1"<<std::endl;
    int t2,low2=0,high2=scount-1;                            //t2  stores index of sid in stud pointer array
    while(low2<=high2){
        t2=(low2+high2)/2;
        if(s[t2]->sid>sid)
        high2=t2-1;
        else if(s[t2]->sid<sid)
        low2=t2+1;
        else
        break;
    }
    if(low2==scount || high2==-1){
        //std::cout<<1<<std::endl;
        return;
    }
    int check=0,count=0;
    //std::cout<<"Checkpoint 2"<<std::endl;
    for(const auto& pre : (c[t1])->prereq){                     //to check if all prerequisites are completed
        for(const auto& completed : (s[t2])->complete){                                    
            if(completed==pre){
                check=1;
                break;
            }
            else
            check=0;
        }
        count++;
        if(check==0)
        break;
    }
    if((check==0 && count!=0)){                                     
        //std::cout<<4<<std::endl;
        return;
    }
    check=0;
    //std::cout<<"Checkpoint 3"<<std::endl;
    for(const auto& enrol : (s[t2])->enrolled){                  //checking if the slot of the course to be enrolled doesn't clash with others
        int t,low=0,high=ccount-1;
        while(low<=high){
            t=(low+high)/2;
            if(c[t]->cid>enrol)
            high=t-1;
            else if(c[t]->cid<enrol)
            low=t+1;
            else
            break;
        }
        if((c[t])->slot==(c[t1])->slot){
            check=1;
            break;
        }
    }
    if(check==1)
    {std::cout<<5<<std::endl;return;}
    (s[t2])->enrolled.push_back(cid);
    (c[t1])->students.push_back(sid);
    (c[t1])->capacity--;
    //std::cout<<"Enrolled "<<sid<<" in "<<cid<<std::endl;
}

int pathlen(courses* c[],std::string ,std::string ,int );

int main(){
    int q,scount=0,ccount=0;
    std::map<std::string, stud*> coursestostud;                             //I thought maps were required but it wasn't
    std::map<std::string, courses*>studtocourses;
    stud* s[1000];                                                           //pointer array of type class stud
    courses* c[1000];                                                         //pointer array of type class courses
    std::cin>>q;
    for(int i=0;i<q;i++){
        std::string choice;
        std::cin>>choice;
        if(choice=="add_student"){
            s[scount]=new stud();
            std::string sname,sid;                                       //student name and student id respectively
            int year,NoOfCourses;                                        //stores year of college of student and no. of courses completed respectively
            int low=0,high=scount,mid;                  
            std::cin>>sid>>sname>>year>>NoOfCourses;
            while(low<=high){
                mid=(low+high)/2;
                if(s[mid]->sid>sid)
                high=mid-1;
                else if(s[mid]->sid<sid)
                low=mid+1;
            }
            int p;
            for(p=scount;p>low+1;p--){                                  //pushing all stud records with sid greater than one to be entered by one
                *s[p]=*s[p-1];                                           //this keeps all records in ascending order of sid
            }

            if(NoOfCourses>0){
                std::string cid[NoOfCourses];                           //stores id of courses completed
                for(int t=0;t<NoOfCourses;t++){
                    std::cin>>cid[t];
                    (s[p]->complete).push_back(cid[t]);
                }
                std::sort((s[p]->complete).begin(),(s[p]->complete).end());
            }
            s[p]->sid=sid;
            s[p]->sname=sname;
            s[p]->year=year;
            scount++;
        }
        else if(choice=="add_course"){
            c[ccount]=new courses();
            std::string cid,cname;                                              //stores course id and course name
            char slot;                                                          //stores the course's slot
            int credits,capacity,NoOfPrereq,check=0,low=0,high=ccount,mid;      //
            std::cin>>cid>>cname>>credits>>capacity>>slot>>NoOfPrereq;
            while(low<=high){                                            
                mid=(low+high)/2;
                if(c[mid]->cid>cid)
                high=mid-1;
                else if(c[mid]->cid<cid)
                low=mid+1;
            }
            int p;
            for(p=ccount;p>low+1;p--){                                  //pushes courses with cid greater than one to be entered by one
                *c[p]=*c[p-1];                                           //so the courses are entered with cid in ascending order   
            }
            if(NoOfPrereq>0){
                std::string prereq[NoOfPrereq];
                for(int i=0;i<NoOfPrereq;i++){
                    std::cin>>prereq[i];
                    int t,low=0,high=ccount-1;
                    while(low<=high){                                            
                        t=(low+high)/2;
                        if(c[t]->cid>prereq[i])
                        high=mid-1;
                        else if(c[mid]->cid<prereq[i])
                        low=mid+1;
                        else
                        break;
                    }
                    for(const auto& pre : c[t]->prereq){
                        if(pre==cid){
                            check=1;
                            break;
                        }
                    }
                    (c[p]->prereq).push_back(prereq[i]);
                }
                std::sort((c[p]->prereq).begin(),(c[p]->prereq).end());       
            }
            if(check==1)
            continue;
            c[p]->cid=cid;
            c[p]->cname=cname;
            c[p]->credits=credits;
            c[p]->capacity=capacity;
            c[p]->slot=slot;
            c[p]->wcount=0;
            ccount++;
        }
        else if(choice=="enroll"){
            std::string sid,cid;
            std::cin>>sid>>cid;
            enroll(c,s,ccount,scount,cid,sid);
            coursestostud[sid]=s[scount-1];                 //data is entered into map but not used
            studtocourses[cid]=c[ccount-1];
        }
        else if(choice=="print"){
            std::string cid;
            std::cin>>cid;
            int t;
            for(t=0;t<ccount;t++){                                      //linear search to get the index of the given cid
                if(c[t]->cid==cid)
                break;
            }
            if(t==ccount)
            std::cout<<"Invalid Course "<<cid<<std::endl;               //if t reaches end of the loop, it means no courses match the cid
            else{
                std::cout<<"Enrolled students in "<<c[t]->cid<<":"<<std::endl;
                for(const auto& stud : c[t]->students){                     //loops through all items in c[t]->students to get all the sids
                    std::cout<<stud<<" "<<std::endl;
                }
            }
        }
        else if(choice=="drop"){
            std::string sid,cid;
            std::cin>>sid>>cid;
            int t1,t2,low=0,high=ccount-1;
            while(low<=high){                               //gets index for cid
                t1=(low+high)/2;
                if(c[t1]->cid>cid)
                high=t1-1;
                else if(c[t1]->cid<cid)
                low=t1+1;
                else
                break;
            }
            low=0;
            high=scount-1;
            while(low<=high){                       //gets index for sid
                t2=(low+high)/2;
                if(s[t2]->sid>sid)
                high=t2-1;
                else if(s[t2]->sid<sid)
                low=t2+1;
                else
                break;
            }
            int check=0;
            for(int t=0;t<(s[t2]->enrolled).size();t++){                //looping through all enrolled courses in s[t2]to find the enrolled cid
                if((s[t2]->enrolled)[t]==cid){
                    int k;
                    for(k=t;k<(s[t2]->enrolled).size()-1;k++){
                        (s[t2]->enrolled)[k]=(s[t2]->enrolled)[k+1];      //pushing all cids in front of the cid to be dropped by one behind
                    }
                    if(k==t)
                    (s[t2]->enrolled).pop_back();                          //special case when the cid to be dropped is the last one
                    check++;
                    break;
                }
            }
            for(int t=0;t<(c[t1]->students).size();t++){                    //similar process for sids
                if((c[t1]->students)[t]==sid){
                    int k;
                    for(k=t;k<(c[t1]->students).size()-1;k++){
                        (c[t1]->students)[k]=(c[t1]->students)[k+1];
                    }
                    if(k==t)
                    (c[t1]->students).pop_back();
                    check++;
                    break;
                }
            }
            if(check==2){                                          //if the course and student ids have been dropped, the next in waitlist is enrolled to that course
                (c[t1]->capacity)++;
                if(c[t1]->wcount>0){
                    sid=(c[t1]->waitlist).front();
                    enroll(c,s,ccount,scount,cid,sid);
                    for(int t=0;t<c[t1]->wcount-1;t++){
                        (c[t1]->waitlist)[t]=(c[t1]->waitlist)[t+1];
                    }
                    (c[t1]->wcount)--;
                }
            }
        }
        // else if(choice=="printwait"){                        //DEBUG
        //     std::string cid;
        //     std::cin>>cid;
        //     int t;
        //     for(t=0;t<ccount;t++){
        //         if(c[t]->cid==cid)
        //         break;
        //     }
        //     for(const auto& wait : c[t]->waitlist){
        //         std::cout<<wait<<std::endl;
        //     }
        // }
        else if(choice=="minEligiblePathlen"){                  //recursive
            std::string cid1,cid2;
            std::cin>>cid1>>cid2;
            
            int t=pathlen(c,cid2,cid1,ccount);
            if(t==0)
            t=-1;
            std::cout<<t<<std::endl;
        }
    }
}
std::string text;                                                          //used to check if the slots are not clashing
int pathlen(courses* c[],std::string cid1,std::string cid2,int ccount){
    int t1,t2,low=0,high=ccount-1;                                          //t1 stores index of cid1,t2 for cid2
    while(low<=high){               
        t1=(low+high)/2;
        if(c[t1]->cid>cid1)
        high=t1-1;
        else if(c[t1]->cid<cid1)
        low=t1+1;
        else
        break;
    }
    low=0;high=ccount-1;
    while(low<=high){
        t2=(low+high)/2;
        if(c[t2]->cid>cid2)
        high=t2-1;
        else if(c[t2]->cid<cid2)
        low=t2+1;
        else
        break;
    }
    if(t1==t2)                                        //base case, if the courses reach the same ones then return 1
    return 1;
    if((c[t1]->prereq).empty())                     //if the course reaches an end of prereqs without reaching the same ones then its a wrong path
    return 0;
    text.append(1,c[t1]->slot);                     //appends current slot to check if the later ones dont clash with this
    text.append(1,c[t2]->slot);
    int t;
    for(const auto& pre1:c[t1]->prereq){                //loops through all prereqs
        int t2,check=0;                                 //t2 stores index of the particular prereq and check is used to check slot clashes in line 349
        t=pathlen(c,pre1,cid2,ccount);                  //recursive to find if its the right path(t=0 if wrong path and t=answer required if right path)
        low=0;high=ccount-1;
        //std::cout<<t<<std::endl;
        while(low<=high){
            t2=(low+high)/2;
            if(c[t2]->cid>pre1)
            high=t2-1;
            else if(c[t2]->cid<pre1)
            low=t2+1;
            else
            break;
        }
        for(int l=0;l<text.length();l++){                //checks for clashes
            if(text[l]==c[t2]->slot)
            {check=1;break;}
        }
        if(check==1)
        {text=text.substr(0,2);continue;}
        text.append(1,c[t1]->slot);                     //append current prereq's slot also to check clash with other prereqs
    }
    if(t!=0)
    return 1+t;                                        //returning 1+t gets the count of the number of recursions that took place in the right path
    else
    return 0;                                           //returning 0 means its a wrong path
}
