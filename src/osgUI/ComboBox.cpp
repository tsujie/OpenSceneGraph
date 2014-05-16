/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2014 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/


#include <osgUI/ComboBox>
#include <osgText/String>
#include <osgText/Font>
#include <osgText/Text>
#include <osg/Notify>
#include <osg/io_utils>

using namespace osgUI;

ComboBox::ComboBox()
{
}

ComboBox::ComboBox(const osgUI::ComboBox& combobox, const osg::CopyOp& copyop):
    Widget(combobox, copyop),
    _items(combobox._items)
{
}

bool ComboBox::handleImplementation(osgGA::EventVisitor* ev, osgGA::Event* event)
{
    // OSG_NOTICE<<"ComboBox::handleImplementation"<<std::endl;

    osgGA::GUIEventAdapter* ea = event->asGUIEventAdapter();
    if (!ea) return false;

    switch(ea->getEventType())
    {
        case(osgGA::GUIEventAdapter::SCROLL):
            OSG_NOTICE<<"Scroll "<<std::endl;
            if (ea->getScrollingMotion()==osgGA::GUIEventAdapter::SCROLL_DOWN)
            {
                OSG_NOTICE<<"Scroll Down`"<<std::endl;
                if (getCurrentItem()<getNumItems()-1) setCurrentItem(getCurrentItem()+1);
            }
            else if (ea->getScrollingMotion()==osgGA::GUIEventAdapter::SCROLL_UP)
            {
                OSG_NOTICE<<"Scroll Up`"<<std::endl;
                if (getCurrentItem()>0) setCurrentItem(getCurrentItem()-1);
            }
            break;
        case(osgGA::GUIEventAdapter::PUSH):
            OSG_NOTICE<<"Button pressed "<<std::endl;
            break;
        case(osgGA::GUIEventAdapter::RELEASE):
            OSG_NOTICE<<"Button release "<<std::endl;
            break;
        default:
            break;
    }

    return false;
}

void ComboBox::enterImplementation()
{
    OSG_NOTICE<<"ComboBox enter"<<std::endl;
}


void ComboBox::leaveImplementation()
{
    OSG_NOTICE<<"ComboBox leave"<<std::endl;
}

void ComboBox::setCurrentItem(unsigned int i)
{
    OSG_NOTICE << "ComboBox::setCurrentItem("<<i<<")"<<std::endl;
    _currentItem = i;
    if (_switch.valid()) _switch->setSingleChildOn(_currentItem);
}


void ComboBox::createGraphicsImplementation()
{

    if (_switch.valid() && _switch->getNumChildren()==_items.size())
    {
        OSG_NOTICE<<"Need to update existing scene graph"<<std::endl;
        _graphicsInitialized = true;
    }
    else
    {
        OSG_NOTICE<<"ComboBox::createGraphicsImplementation()"<<std::endl;

        Widget::createGraphicsImplementation();

        Style* style = (getStyle()!=0) ? getStyle() : Style::instance().get();

        _switch = new osg::Switch;
        addChild(_switch.get());

        if (!_items.empty())
        {
            for(Items::iterator itr = _items.begin();
                itr != _items.end();
                ++itr)
            {
                Item* item = itr->get();
                OSG_NOTICE<<"Creating item "<<item->getText()<<", "<<item->getColor()<<std::endl;
                osg::ref_ptr<osg::Group> group = new osg::Group;
                if (item->getColor().a()!=0.0f) group->addChild( style->createPanel(_extents, item->getColor()) );
                if (!item->getText().empty()) group->addChild( style->createText(_extents, getAlignmentSettings(), getTextSettings(), item->getText()) );
                _switch->addChild(group.get());
            }
        }
        else
        {
            _switch->addChild( style->createPanel(_extents, osg::Vec4(1.0f,1.0f,1.0f,1.0f)) );
        }

        _switch->setSingleChildOn(_currentItem);
    }
}