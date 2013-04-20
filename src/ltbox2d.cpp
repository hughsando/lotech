/* Copyright (C) 2010 Ian MacLarty */

#include "lt.h"

LT_INIT_IMPL(ltbox2d)

LTWorld::LTWorld() {
    world = new b2World(b2Vec2(0.0f, 0.0f));
    world->SetAllowSleeping(true);
    scale = 1.0f;
}

LTWorld::~LTWorld() {
    delete world;
}

LTBody::LTBody(LTWorld *world, const b2BodyDef *def) {
    LTBody::world = world;
    body = world->world->CreateBody(def);
    body->SetUserData(this);
}

void LTBody::destroy() {
    if (body != NULL) { // NULL means the body was already destroyed.
        // Invalidate fixture wrappers.
        b2Fixture *f = body->GetFixtureList();
        while (f != NULL) {
            LTFixture *ud = (LTFixture*)f->GetUserData();
            ud->fixture = NULL;
            f = f->GetNext();
        }
        // Invalidate joint wrappers.
        b2JointEdge *j = body->GetJointList();
        while (j != NULL) {
            LTJoint *ud = (LTJoint*)j->joint->GetUserData();
            ud->joint = NULL;
            j = j->next;
        }

        world->world->DestroyBody(body);
        world = NULL;
        body = NULL;
    }
}

void LTBody::draw() {
    if (body != NULL) {
        b2Vec2 pos = body->GetPosition();
        ltTranslate(pos.x, pos.y, 0.0f);
        ltRotate(body->GetAngle() * LT_DEGREES_PER_RADIAN, 0.0f, 0.0f, 1.0f);
        b2Fixture *fixture = body->GetFixtureList();
        while (fixture != NULL) {
            LTFixture *f = (LTFixture*)fixture->GetUserData();
            f->draw();
            fixture = fixture->GetNext();
        }
    }
}

//bool LTBody::containsPoint(LTfloat x, LTfloat y) {
//    b2Vec2 p = b2Vec2(x, y);
//    b2Fixture *fixture = body->GetFixtureList();
//    while (fixture != NULL) {
//        if (fixture->TestPoint(p)) {
//            return true;
//        }
//        fixture = fixture->GetNext();
//    }
//    return false;
//}

LTFixture::LTFixture(LTBody *body, const b2FixtureDef *def) {
    LTFixture::body = body;
    if (body->body != NULL) {
        fixture = body->body->CreateFixture(def);
        fixture->SetUserData(this);
    } else {
        // User tried to add fixture to destroyed body.
        fixture = NULL;
    }
}

void LTFixture::destroy() {
    if (fixture != NULL) { // NULL means the fixture was already destroyed.
        body->body->DestroyFixture(fixture);
        fixture = NULL;
    }
}

void LTFixture::draw() {
    if (fixture != NULL) {
        b2Shape *shape = fixture->GetShape();
        switch (shape->m_type) {
            case b2Shape::e_edge:
                break;
            case b2Shape::e_chain:
                break;
            case b2Shape::e_circle: {
                b2CircleShape *circle = (b2CircleShape *)shape;
                ltPushMatrix();
                ltPushTint(1.0f, 1.0f, 1.0f, 0.5f);
                ltTranslate(circle->m_p.x, circle->m_p.y, 0.0f);
                ltScale(circle->m_radius, circle->m_radius, 1.0f);
                ltDrawUnitCircle();
                ltPopTint();
                ltPopMatrix();
                break;
            }
            case b2Shape::e_polygon: {
                b2PolygonShape *poly = (b2PolygonShape *)shape;
                ltPushTint(1.0f, 1.0f, 1.0f, 0.5f);
                ltDrawPoly((LTfloat *)poly->m_vertices, poly->m_vertexCount);
                ltPopTint();
                for (int i = 0; i < poly->m_vertexCount - 1; i++) {
                    ltVertexPointer(2, LT_VERT_DATA_TYPE_FLOAT, 0, &poly->m_vertices[i]);
                    ltDrawArrays(LT_DRAWMODE_LINE_STRIP, 0, 2);
                }
                b2Vec2 final_line[2];
                final_line[0] = poly->m_vertices[poly->m_vertexCount - 1];
                final_line[1] = poly->m_vertices[0];
                ltVertexPointer(2, LT_VERT_DATA_TYPE_FLOAT, 0, final_line);
                ltDrawArrays(LT_DRAWMODE_LINE_STRIP, 0, 2);
                break;
            }
            case b2Shape::e_typeCount:
                break;
        }
    }
}

LTJoint::LTJoint(LTWorld *world, const b2JointDef *def) {
    LTJoint::world = world;
    LTJoint::joint = world->world->CreateJoint(def);
    LTJoint::joint->SetUserData(this);
}

void LTJoint::destroy() {
    if (joint != NULL) {
        world->world->DestroyJoint(joint);
        joint = NULL;
    }
}

LTBodyTracker::LTBodyTracker(LTBody *body, LTSceneNode *child,
    bool viewport_mode, bool track_rotation,
    LTfloat min_x, LTfloat max_x, LTfloat min_y, LTfloat max_y, LTfloat snap_to)
{
    LTBodyTracker::viewport_mode = viewport_mode;
    LTBodyTracker::track_rotation = track_rotation;
    LTBodyTracker::min_x = min_x;
    LTBodyTracker::max_x = max_x;
    LTBodyTracker::min_y = min_y;
    LTBodyTracker::max_y = max_y;
    LTBodyTracker::snap_to = snap_to;
    LTWorld *w = body->world;
    if (w != NULL) {
        LTBodyTracker::scale = w->scale;
    } else {
        LTBodyTracker::scale = 1.0f;
    }
    LTBodyTracker::body = body;
}

void LTBodyTracker::draw() {
    static LTfloat rmat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    b2Body *b = body->body;
    if (b != NULL) {
        const b2Transform b2t = b->GetTransform();
        LTfloat x = b2t.p.x * scale;
        LTfloat y = b2t.p.y * scale;
        if (snap_to > 0.0f) {
            x = roundf(x / snap_to) * snap_to;
            y = roundf(y / snap_to) * snap_to;
        }
        if (x < min_x) {
            x = min_x;
        } else if (x > max_x) {
            x = max_x;
        }
        if (y < min_y) {
            y = min_y;
        } else if (y > max_y) {
            y = max_y;
        }
        if (viewport_mode) {
            if (track_rotation) {
                rmat[0] = b2t.q.c;
                rmat[1] = -b2t.q.s;
                rmat[4] = b2t.q.s;
                rmat[5] = b2t.q.c;
                ltMultMatrix(rmat);
            }
            ltTranslate(-x, -y, 0.0f);
        } else {
            ltTranslate(x, y, 0.0f);
            if (track_rotation) {
                rmat[0] = b2t.q.c;
                rmat[1] = b2t.q.s;
                rmat[4] = -b2t.q.s;
                rmat[5] = b2t.q.c;
                ltMultMatrix(rmat);
            }
        }
        child->draw();
    }
}

bool LTBodyTracker::inverse_transform(LTfloat *x, LTfloat *y) {
    b2Body *b = body->body;
    if (b != NULL) {
        if (viewport_mode) {
            // XXX viewport mode NYI
            return false;
        }
        LTfloat angle = b->GetAngle();
        b2Vec2 pos = b->GetPosition();
        *x = *x - pos.x;
        *y = *y - pos.y;
        LTfloat x1, y1;
        LTfloat s = sinf(angle);
        LTfloat c = cosf(angle);
        x1 = c * (*x) - s * (*y);
        y1 = s * (*x) + c * (*y);
        *x = x1;
        *y = y1;
        return true;
    } else {
        return false;
    }
}

bool ltCheckB2Poly(const b2Vec2* vs, int32 count) {
    // This code copied from Box2D (b2PolygonShape.cpp, ComputeCentroid).

    if (count < 2) {
        return false;
    }
    if (count == 2) {
        return true;
    }

    b2Vec2 c;
    c.Set(0.0f, 0.0f);
    float32 area = 0.0f;
    b2Vec2 pRef(0.0f, 0.0f);

    const float32 inv3 = 1.0f / 3.0f;

    for (int32 i = 0; i < count; ++i) {
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = vs[i];
        b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];

        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;

        float32 D = b2Cross(e1, e2);

        float32 triangleArea = 0.5f * D;
        area += triangleArea;

        c += triangleArea * inv3 * (p1 + p2 + p3);
    }

    return area > b2_epsilon;
}

//---------------------------------------------------

LTWorld *lt_expect_LTWorld(lua_State *L, int arg);
LTBody *lt_expect_LTBody(lua_State *L, int arg);
LTFixture *lt_expect_LTFixture(lua_State *L, int arg);
LTJoint *lt_expect_LTJoint(lua_State *L, int arg);

void *lt_alloc_LTBody(lua_State *L);

static LTfloat get_world_gx(LTObject *obj) {
    return ((LTWorld*)obj)->world->GetGravity().x;
}

static LTfloat get_world_gy(LTObject *obj) {
    return ((LTWorld*)obj)->world->GetGravity().y;
}

static void set_world_gx(LTObject *obj, LTfloat val) {
    LTWorld *w = (LTWorld*)obj;
    b2Vec2 g = w->world->GetGravity();
    w->world->SetGravity(b2Vec2(val, g.y));
}

static void set_world_gy(LTObject *obj, LTfloat val) {
    LTWorld *w = (LTWorld*)obj;
    b2Vec2 g = w->world->GetGravity();
    w->world->SetGravity(b2Vec2(g.x, val));
}

static LTbool get_world_auto_clear_forces(LTObject *obj) {
    LTWorld *w = (LTWorld*)obj;
    return w->world->GetAutoClearForces();
}

static void set_world_auto_clear_forces(LTObject *obj, LTbool val) {
    LTWorld *w = (LTWorld*)obj;
    w->world->SetAutoClearForces(val);
}

static int world_step(lua_State *L) {
    int num_args = ltLuaCheckNArgs(L, 2); 
    LTWorld *world = lt_expect_LTWorld(L, 1);
    LTfloat time_step = luaL_checknumber(L, 2);
    int velocity_iterations = 8;
    int position_iterations = 3;
    if (num_args > 2) {
        velocity_iterations = luaL_checkinteger(L, 3);
    }
    if (num_args > 3) {
        position_iterations = luaL_checkinteger(L, 4);
    }
    world->world->Step(time_step, velocity_iterations, position_iterations);
    return 0;
}

static int new_body(lua_State *L) {
    ltLuaCheckNArgs(L, 2);
    LTWorld *world = lt_expect_LTWorld(L, 1);

    // Second argument is a table of body properties.
    b2BodyDef body_def;

    lua_getfield(L, 2, "type");
    if (!lua_isnil(L, -1)) {
        const char *type = luaL_checkstring(L, -1);
        b2BodyType btype;
        if (strcmp(type, "dynamic") == 0) {
            btype = b2_dynamicBody;
        } else if (strcmp(type, "static") == 0) {
            btype = b2_staticBody;
        } else if (strcmp(type, "kinematic") == 0) {
            btype = b2_kinematicBody;
        } else {
            return luaL_error(L, "Unknown body type: %s", type);
        }
        body_def.type = btype;
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "x");
    body_def.position.x = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    body_def.position.y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "angle");
    if (!lua_isnil(L, -1)) {
        body_def.angle = LT_RADIANS_PER_DEGREE * luaL_checknumber(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "vx");
    body_def.linearVelocity.x = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "vy");
    body_def.linearVelocity.y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 2, "angular_velocity");
    if (!lua_isnil(L, -1)) {
        body_def.angularVelocity = LT_RADIANS_PER_DEGREE * luaL_checknumber(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "damping");
    if (!lua_isnil(L, -1)) {
        body_def.linearDamping = luaL_checknumber(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "angular_damping");
    if (!lua_isnil(L, -1)) {
        body_def.angularDamping = LT_RADIANS_PER_DEGREE * luaL_checknumber(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "can_sleep");
    if (!lua_isnil(L, -1)) {
        body_def.allowSleep = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "awake");
    if (!lua_isnil(L, -1)) {
        body_def.awake = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "fixed_rotation");
    if (!lua_isnil(L, -1)) {
        body_def.fixedRotation = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "bullet");
    if (!lua_isnil(L, -1)) {
        body_def.bullet = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "active");
    if (!lua_isnil(L, -1)) {
        body_def.active = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    LTBody *body = new (lt_alloc_LTBody(L)) LTBody(world, &body_def);
    body->world_ref = ltLuaAddRef(L, 1, -1); // Add reference from world to body.
    ltLuaAddRef(L, -1, 1); // Add reference from body to world.
    return 1;
}

static LTfloat get_body_x(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetPosition().x;
    } else {
        return 0.0f;
    }
}

static void set_body_x(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b2Vec2 pos = b->body->GetPosition();
        LTfloat angle = b->body->GetAngle();
        b->body->SetTransform(b2Vec2(val, pos.y), angle);
    }
}

static LTfloat get_body_y(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetPosition().y;
    } else {
        return 0.0f;
    }
}

static void set_body_y(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b2Vec2 pos = b->body->GetPosition();
        LTfloat angle = b->body->GetAngle();
        b->body->SetTransform(b2Vec2(pos.x, val), angle);
    }
}

static LTfloat get_body_angle(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetAngle();
    } else {
        return 0.0f;
    }
}

static void set_body_angle(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b2Vec2 pos = b->body->GetPosition();
        b->body->SetTransform(pos, val * LT_RADIANS_PER_DEGREE);
    }
}

static LTfloat get_body_vx(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetLinearVelocity().x;
    } else {
        return 0.0f;
    }
}

static void set_body_vx(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b2Vec2 v = b->body->GetLinearVelocity();
        b->body->SetLinearVelocity(b2Vec2(val, v.y));
    }
}

static LTfloat get_body_vy(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetLinearVelocity().y;
    } else {
        return 0.0f;
    }
}

static void set_body_vy(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b2Vec2 v = b->body->GetLinearVelocity();
        b->body->SetLinearVelocity(b2Vec2(v.x, val));
    }
}

static LTfloat get_body_angular_velocity(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        return b->body->GetAngularVelocity();
    } else {
        return 0.0f;
    }
}

static LTbool get_body_destroyed(LTObject *obj) {
    LTBody *b = (LTBody*)obj;
    return b->body == NULL;
}

static void set_body_angular_velocity(LTObject *obj, LTfloat val) {
    LTBody *b = (LTBody*)obj;
    if (b->body != NULL) {
        b->body->SetAngularVelocity(val * LT_RADIANS_PER_DEGREE);
    }
}

static int body_apply_force(lua_State *L) {
    int num_args = ltLuaCheckNArgs(L, 3);
    LTBody *body = lt_expect_LTBody(L, 1);
    if (body->body != NULL) {
        b2Vec2 force;
        b2Vec2 pos;
        force.x = luaL_checknumber(L, 2);
        force.y = luaL_checknumber(L, 3);
        if (num_args >= 5) {
            pos.x = luaL_checknumber(L, 4);
            pos.y = luaL_checknumber(L, 5);
        } else {
            pos = body->body->GetWorldCenter();
        }
        body->body->ApplyForce(force, pos);
    }
    return 0;
}

static int body_apply_torque(lua_State *L) {
    ltLuaCheckNArgs(L, 2);
    LTBody *body = lt_expect_LTBody(L, 1);
    if (body->body != NULL) {
        body->body->ApplyTorque(luaL_checknumber(L, 2));
    }
    return 0;
}

static int body_apply_impulse(lua_State *L) {
    int num_args = ltLuaCheckNArgs(L, 3);
    LTBody *body = lt_expect_LTBody(L, 1);
    if (body->body != NULL) {
        b2Vec2 force;
        b2Vec2 pos;
        force.x = luaL_checknumber(L, 2);
        force.y = luaL_checknumber(L, 3);
        if (num_args >= 5) {
            pos.x = luaL_checknumber(L, 4);
            pos.y = luaL_checknumber(L, 5);
        } else {
            pos = body->body->GetWorldCenter();
        }
        body->body->ApplyLinearImpulse(force, pos);
    }
    return 0;
}

static int body_apply_angular_impulse(lua_State *L) {
    ltLuaCheckNArgs(L, 2);
    LTBody *body = lt_expect_LTBody(L, 1);
    if (body->body != NULL) {
        body->body->ApplyAngularImpulse(luaL_checknumber(L, 2));
    }
    return 0;
}

static int destroy_body(lua_State *L) {
    ltLuaCheckNArgs(L, 1);
    LTBody *body = lt_expect_LTBody(L, 1);
    b2Body *b = body->body;
    if (b != NULL) {
        body->destroy();
        ltLuaGetRef(L, 1, body->world_ref);
        ltLuaDelRef(L, -1, 1); // Remove reference from world to body
                               // so body can be GC'd.
        lua_pop(L, 1);
    }
    return 0;
}


LT_REGISTER_TYPE(LTWorld, "box2d.World", "lt.Object");
LT_REGISTER_PROPERTY_FLOAT(LTWorld, gx, get_world_gx, set_world_gx);
LT_REGISTER_PROPERTY_FLOAT(LTWorld, gx, get_world_gy, set_world_gy);
LT_REGISTER_PROPERTY_BOOL(LTWorld, auto_clear_forces, get_world_auto_clear_forces, set_world_auto_clear_forces);
LT_REGISTER_FIELD_FLOAT(LTWorld, scale);
LT_REGISTER_METHOD(LTWorld, Step, world_step);
LT_REGISTER_METHOD(LTWorld, Body, new_body);

LT_REGISTER_TYPE(LTBody, "box2d.Body", "lt.SceneNode");
LT_REGISTER_PROPERTY_FLOAT(LTBody, x, get_body_x, set_body_x);
LT_REGISTER_PROPERTY_FLOAT(LTBody, y, get_body_y, set_body_y);
LT_REGISTER_PROPERTY_FLOAT(LTBody, angle, get_body_angle, set_body_angle);
LT_REGISTER_PROPERTY_FLOAT(LTBody, vx, get_body_vx, set_body_vx);
LT_REGISTER_PROPERTY_FLOAT(LTBody, vy, get_body_vy, set_body_vy);
LT_REGISTER_PROPERTY_FLOAT(LTBody, angular_velocity, get_body_angular_velocity, set_body_angular_velocity);
LT_REGISTER_PROPERTY_BOOL(LTBody, destroyed, get_body_destroyed, NULL);
LT_REGISTER_METHOD(LTBody, Force, body_apply_force);
LT_REGISTER_METHOD(LTBody, Torque, body_apply_torque);
LT_REGISTER_METHOD(LTBody, Impulse, body_apply_impulse);
LT_REGISTER_METHOD(LTBody, AngularImpulse, body_apply_angular_impulse);
LT_REGISTER_METHOD(LTBody, Destroy, destroy_body);