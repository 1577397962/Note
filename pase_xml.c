#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define max 10
typedef struct people{
    int id;
    char sorce[32];
    char name[32];
}people;

static xmlNodePtr create_node(struct people *p)
{
    assert(p);
    
    xmlNodePtr node = NULL;
    char id[32];
    node = xmlNewNode(NULL,BAD_CAST"INFO");
    if(node == NULL)
    {
        fprintf(stderr,"failed to create new node\n");
        return NULL;
    }
    
    snprintf(id,32,"%d",p->id);
    xmlNewProp(node,BAD_CAST"id",(xmlChar *)id);

    xmlNewChild(node,NULL,BAD_CAST"sorce",(xmlChar *)p->sorce);
    xmlNewChild(node,NULL,BAD_CAST"name",(xmlChar *)p->name);
    
    return node;
}
static int add_root_xml_node(xmlNodePtr root_node,struct people *p)
{
    xmlNodePtr node = NULL;
    if (!p)
    {
        fprintf(stderr,"failed to malloc!");
        return -1;
    }
    
    node = create_node(p);
    if(!node)
    {
        fprintf(stderr,"failed create node");
        goto ERROR;
    }
    
    xmlAddChild(root_node,node);
    return 0;
ERROR:
    if(node)
    {
        free(node);
    }
    return -1;
}

static int create_xml(char *file_xml_name,struct people *p)
{
    assert(file_xml_name);
    
    xmlDocPtr doc = NULL;
    xmlNodePtr node = NULL;
    doc = xmlNewDoc(BAD_CAST"1.0");
    if(doc == NULL)
    {
        fprintf(stderr,"failed to new doc.\n");
        return -1;
    }
    printf("%s create success\n",file_xml_name);
    
    node = xmlNewNode(NULL,BAD_CAST"student");
    if(node == NULL)
    {
        fprintf(stderr,"failed to new node.\n");
        goto FAILED;
    }
    
    xmlDocSetRootElement(doc,node);

    if(add_root_xml_node(node,p) != 0)
    {
        fprintf(stderr,"failed to create node.\n");
        goto FAILED;
    }
    
    xmlSaveFormatFileEnc(file_xml_name,doc,"UTF-8",1);
    xmlFreeDoc(doc);
    return 0;
    
FAILED:
    if(doc)
    {
        xmlFreeDoc(doc);
    }
    printf("xml error\n");
    return -1;
    
}

static int add_node(char *filename,struct people *p)
{
    assert(filename);

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr node = NULL;
    
    doc = xmlParseFile(filename);
    if(doc == NULL)
    {
        fprintf(stderr,"failed to parser xml file:%s\n",filename);
        return -1;
    }

    root_node = xmlDocGetRootElement(doc);
    if(root_node == NULL)
    {
        fprintf(stderr,"failed to get root node.\n");
        goto FAILED;
    }

    if(add_root_xml_node(root_node,p) != 0)
    {
        fprintf(stderr,"failed to add a new node.\n");
        goto FAILED;
    }

    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    // xmlCleanupParser();
    // xmlMemoryDump();
    xmlFreeDoc(doc);

    return 0;
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }
 
    return -1;
}

static int parse_xml(xmlDocPtr doc,xmlNodePtr cur)
{
    assert(doc || cur);

    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while(cur)
    {
        if (!xmlStrcmp(cur->name,(xmlChar *)"name")){
            key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
            printf("name: %s\t",key);
            xmlFree(key);
        }
        if (!xmlStrcmp(cur->name,(xmlChar *)"sorce")){
            key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
            printf("sorce: %s\t",key);
            xmlFree(key);
        }
        printf("\n");
        cur = cur->next;
    }
    return 0;
}

static int parse_to_xml(char *filename)
{
    assert(filename);

    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *id;

    doc = xmlParseFile(filename);
    if(doc == NULL)
    {
        fprintf(stderr,"failed to parse %s xml.\n",filename);
        goto FAILED;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        fprintf(stderr,"root is empty.\n");
        goto FAILED;
    }
 
    if ((xmlStrcmp(cur->name, (xmlChar *)"student")))
    {
        fprintf(stderr, "The root is not student.\n");
        goto FAILED;
    }
    cur = cur->xmlChildrenNode;
    while(cur != NULL)
    {
        if (!xmlStrcmp(cur->name,(xmlChar *)"INFO")){
            id = xmlGetProp(cur,"id");
            printf("id: %s\t",id);
            parse_xml(doc,cur);
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
    return 0;
FAILED:
    if (doc) {
        xmlFreeDoc(doc);
    }
    return -1;
}

int main(int argc,char *argv[])
{
    char *filename = "config.xml";
    struct people *p = NULL;
    for (int i = 1;i < max; i++)
    {
        p = (struct people *)malloc(sizeof(people));
        p->id = i;
        //strcpy(p->sorce,sorce);
        //strcpy(p->name,name);
        snprintf(p->name,32,"%s_%d","lijinze",i);
        snprintf(p->sorce,32,"%s.%d","100",i);
        
        if (access(filename,F_OK)){
            create_xml(filename,p);
        }
        else{
            add_node(filename,p);
        }
        free(p);
        p = NULL;
    }   
    if (parse_to_xml(filename) != 0)
    {
        fprintf(stderr,"failed to parse config.xml\n");
        return -1;
    }
    return 0;
}