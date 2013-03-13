var NAVTREE =
[
  [ "Machanguitos", "index.html", [
    [ "Machanguitos", "index.html", null ],
    [ "Modules", "modules.html", [
      [ "Main classes to run the Multi-Agent System.", "group__Engine.html", null ],
      [ "Multi-Agent definitions.", "group__Agent.html", null ],
      [ "Utils, File functions and commons stuff.", "group__Util.html", null ],
      [ "Configuration classes.", "group__Config.html", null ]
    ] ],
    [ "Class List", "annotated.html", [
      [ "Agent::AgentClass", "classAgent_1_1AgentClass.html", null ],
      [ "Agent::AgentFactory", "classAgent_1_1AgentFactory.html", null ],
      [ "Agent::AgentInstance", "classAgent_1_1AgentInstance.html", null ],
      [ "Engine::Client", "classEngine_1_1Client.html", null ],
      [ "Engine::ClientLocal", "classEngine_1_1ClientLocal.html", null ],
      [ "Engine::ClientRemote", "classEngine_1_1ClientRemote.html", null ],
      [ "Engine::MPIWorker", "classEngine_1_1MPIWorker.html", null ],
      [ "Engine::Server", "classEngine_1_1Server.html", null ],
      [ "Singleton< T >", "classSingleton.html", null ],
      [ "Util::ScriptValue", "classUtil_1_1ScriptValue.html", null ],
      [ "Util::ScriptValue::Value", "unionUtil_1_1ScriptValue_1_1Value.html", null ]
    ] ],
    [ "Class Index", "classes.html", null ],
    [ "Class Hierarchy", "hierarchy.html", [
      [ "Agent::AgentClass", "classAgent_1_1AgentClass.html", null ],
      [ "Agent::AgentInstance", "classAgent_1_1AgentInstance.html", null ],
      [ "Engine::Client", "classEngine_1_1Client.html", [
        [ "Engine::ClientLocal", "classEngine_1_1ClientLocal.html", null ],
        [ "Engine::ClientRemote", "classEngine_1_1ClientRemote.html", null ]
      ] ],
      [ "Engine::MPIWorker", "classEngine_1_1MPIWorker.html", null ],
      [ "Singleton< T >", "classSingleton.html", null ],
      [ "Singleton< AgentFactory >", "classSingleton.html", [
        [ "Agent::AgentFactory", "classAgent_1_1AgentFactory.html", null ]
      ] ],
      [ "Singleton< Server >", "classSingleton.html", [
        [ "Engine::Server", "classEngine_1_1Server.html", null ]
      ] ],
      [ "Util::ScriptValue", "classUtil_1_1ScriptValue.html", null ],
      [ "Util::ScriptValue::Value", "unionUtil_1_1ScriptValue_1_1Value.html", null ]
    ] ],
    [ "Class Members", "functions.html", null ],
    [ "File List", "files.html", [
      [ "doc/doxydoc", null, null ],
      [ "src/agentclass.cpp", null, null ],
      [ "src/agentclass.h", null, null ],
      [ "src/agentfactory.cpp", null, null ],
      [ "src/agentfactory.h", null, null ],
      [ "src/agentinstance.cpp", null, null ],
      [ "src/agentinstance.h", null, null ],
      [ "src/client.cpp", null, null ],
      [ "src/client.h", null, null ],
      [ "src/clientlocal.cpp", null, null ],
      [ "src/clientlocal.h", null, null ],
      [ "src/clientremote.cpp", null, null ],
      [ "src/clientremote.h", null, null ],
      [ "src/configlib.cpp", null, null ],
      [ "src/configlib.h", null, null ],
      [ "src/main.cpp", null, null ],
      [ "src/mpitags.h", null, null ],
      [ "src/mpiworker.cpp", null, null ],
      [ "src/mpiworker.h", null, null ],
      [ "src/scriptvalue.cpp", null, null ],
      [ "src/scriptvalue.h", null, null ],
      [ "src/server.cpp", null, null ],
      [ "src/server.h", null, null ],
      [ "src/singleton.h", null, null ],
      [ "src/util.cpp", null, null ],
      [ "src/util.h", null, null ]
    ] ],
    [ "Directories", "dirs.html", [
      [ "doc", "dir_9387a64caad1551538a0d019b67b2c32.html", null ],
      [ "src", "dir_a7a96e634516aa283b5ad1f54e4e9acb.html", null ]
    ] ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

